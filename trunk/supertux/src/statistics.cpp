//
//  SuperTux -  A Jump'n Run
//  Copyright (C) 2004 Ricardo Cruz <rick2@aeiou.pt>
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
// 
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
//  02111-1307, USA.

#include "utils/lispreader.h"
#include "utils/lispwriter.h"
#include "statistics.h"
#include "video/drawing_context.h"
#include "app/gettext.h"
#include "resources.h"

Statistics global_stats;

std::string
stat_name_to_string(int stat_enum)
{
  switch(stat_enum)
    {
    case SCORE_STAT:
      return "score";
    case BADGUYS_SQUISHED_STAT:
      return "badguys-squished";
    case SHOTS_STAT:
      return "shots";
    case TIME_NEEDED_STAT:
      return "time-needed";
    case JUMPS_STAT:
      return "jumps";
    }
}

int
my_min(int a, int b)
{
if(a == -1)
  return b;
if(b == -1)
  return a;
return std::min(a, b);
}

Statistics::Statistics()
{
  timer.init(true);
  display_stat = 1;

  for(int i = 0; i < NUM_STATS; i++)
    stats[i] = -1;
}

Statistics::~Statistics()
{
}

void
Statistics::parse(LispReader& reader)
{
  for(int i = 0; i < NUM_STATS; i++)
    reader.read_int(stat_name_to_string(i).c_str(), stats[i]);
}

void
Statistics::write(LispWriter& writer)
{
  for(int i = 0; i < NUM_STATS; i++)
    writer.write_int(stat_name_to_string(i), stats[i]);
}

#define TOTAL_DISPLAY_TIME 3400
#define FADING_TIME         600

void
Statistics::draw_worldmap_info(DrawingContext& context)
{
  if(stats[SCORE_STAT] == -1)  // not initialized yet
    return;

  if(!timer.check())
    {
    timer.start(TOTAL_DISPLAY_TIME);
    display_stat++;
    if(display_stat >= NUM_STATS)
      display_stat = 1;
    }

  int alpha;
  if(timer.get_gone() < FADING_TIME)
    alpha = timer.get_gone() * 255 / FADING_TIME;
  else if(timer.get_left() < FADING_TIME)
    alpha = timer.get_left() * 255 / FADING_TIME;
  else
    alpha = 255;

  char str[128];

  context.draw_text(white_small_text, _("Level Statistics"), Vector(550, 490), LAYER_GUI);

  sprintf(str, _("Max score: %d"), stats[SCORE_STAT]);
  context.draw_text(white_small_text, str, Vector(560, 506), LAYER_GUI);

  if(display_stat == BADGUYS_SQUISHED_STAT)
    sprintf(str, _("Max fragging: %d"), stats[BADGUYS_SQUISHED_STAT]);
  else if(display_stat == SHOTS_STAT)
    sprintf(str, _("Min shots: %d"), stats[SHOTS_STAT]);
  else if(display_stat == TIME_NEEDED_STAT)
    sprintf(str, _("Min time needed: %d"), stats[TIME_NEEDED_STAT]);
  else// if(display_stat == JUMPS_STAT)
    sprintf(str, _("Min jumps: %d"), stats[JUMPS_STAT]);

  context.draw_text(white_small_text, str, Vector(560, 522), LAYER_GUI, NONE_EFFECT, alpha);
}

void
Statistics::draw_message_info(DrawingContext& context, std::string title)
{
  if(stats[SCORE_STAT] == -1)  // not initialized yet
    return;

  context.draw_text_center(gold_text, title, Vector(0, 400), LAYER_GUI);

  char str[128];
  for(int i = 0; i < NUM_STATS; i++)
    {
    if(i == SCORE_STAT)
      sprintf(str, _("Max score: %d"), stats[SCORE_STAT]);
    else if(i == BADGUYS_SQUISHED_STAT)
      sprintf(str, _("Max fragging: %d"), stats[BADGUYS_SQUISHED_STAT]);
    else if(i == SHOTS_STAT)
      sprintf(str, _("Min shots: %d"), stats[SHOTS_STAT]);
    else if(i == TIME_NEEDED_STAT)
      sprintf(str, _("Min time needed: %d"), stats[TIME_NEEDED_STAT]);
    else// if(i == JUMPS_STAT)
      sprintf(str, _("Min jumps: %d"), stats[JUMPS_STAT]);

    context.draw_text_center(white_text, str, Vector(0, 430 + i*22), LAYER_GUI);
    }
}

void
Statistics::add_points(int stat, int points)
{
  stats[stat] += points;
}

int
Statistics::get_points(int stat)
{
  return stats[stat];
}

void
Statistics::set_points(int stat, int points)
{
  stats[stat] = points;
}

void
Statistics::reset()
{
  for(int i = 0; i < NUM_STATS; i++)
    stats[i] = 0;
}

void
Statistics::merge(Statistics& stats_)
{
  stats[SCORE_STAT] = std::max(stats[SCORE_STAT], stats_.stats[SCORE_STAT]);
  if(stats[JUMPS_STAT] != -1)
    stats[JUMPS_STAT] = my_min(stats[JUMPS_STAT], stats_.stats[JUMPS_STAT]);
  stats[BADGUYS_SQUISHED_STAT] =
    std::max(stats[BADGUYS_SQUISHED_STAT], stats_.stats[BADGUYS_SQUISHED_STAT]);
  stats[SHOTS_STAT] = my_min(stats[SHOTS_STAT], stats_.stats[SHOTS_STAT]);
  stats[TIME_NEEDED_STAT] =
    my_min(stats[TIME_NEEDED_STAT], stats_.stats[TIME_NEEDED_STAT]);
}

void
Statistics::operator+=(const Statistics& stats_)
{
  for(int i = 0; i < NUM_STATS; i++)
    stats[i] += stats_.stats[i];
}
