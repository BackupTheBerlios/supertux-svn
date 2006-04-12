#include "sound_manager.hpp"

#include <stdexcept>
#include <iostream>
#include <sstream>
#include <memory>

#include "sound_file.hpp"
#include "sound_source.hpp"
#include "stream_sound_source.hpp"
#include "log.hpp"

SoundManager* sound_manager = 0;

SoundManager::SoundManager()
  : device(0), context(0), sound_enabled(false), music_source(0),
    music_enabled(true)
{
  try {
    device = alcOpenDevice(0);
    if(device == 0) {
      print_openal_version();
      throw std::runtime_error("Couldn't open audio device.");
    }

    int attributes[] = { 0 };
    context = alcCreateContext(device, attributes);
    check_alc_error("Couldn't create audio context: ");
    alcMakeContextCurrent(context);
    check_alc_error("Couldn't select audio context: ");

    check_al_error("Audio error after init: ");
    sound_enabled = true;
  } catch(std::exception& e) {
    device = 0;
    context = 0;
    log_warning << "Couldn't initialize audio device:" << e.what() << std::endl;
    print_openal_version();
    throw e;
  }
}

SoundManager::~SoundManager()
{
  delete music_source;

  for(SoundSources::iterator i = sources.begin(); i != sources.end(); ++i) {
    delete *i;
  }

  for(SoundBuffers::iterator i = buffers.begin(); i != buffers.end(); ++i) {
    ALuint buffer = i->second;
    alDeleteBuffers(1, &buffer);
  }

  if(context != 0) {
    alcDestroyContext(context);
  }
  if(device != 0) {
    alcCloseDevice(device);
  }
}

ALuint
SoundManager::load_file_into_buffer(SoundFile* file)
{
  ALenum format = get_sample_format(file);
  ALuint buffer;
  alGenBuffers(1, &buffer);
  check_al_error("Couldn't create audio buffer: ");
  char* samples = new char[file->size];
  try {
    file->read(samples, file->size);
    alBufferData(buffer, format, samples,
        static_cast<ALsizei> (file->size),
        static_cast<ALsizei> (file->rate));
    check_al_error("Couldn't fill audio buffer: ");
  } catch(...) {
    delete[] samples;
    throw;
  }
  delete[] samples;

  return buffer;
}

SoundSource*
SoundManager::create_sound_source(const std::string& filename)
{
  if(!sound_enabled)
    return 0;

  ALuint buffer;
  
  // reuse an existing static sound buffer            
  SoundBuffers::iterator i = buffers.find(filename);
  if(i != buffers.end()) {
    buffer = i->second;
  } else {
    // Load sound file
    std::auto_ptr<SoundFile> file (load_sound_file(filename));

    if(file->size < 100000) {
      buffer = load_file_into_buffer(file.get());
      buffers.insert(std::make_pair(filename, buffer));
    } else {
      StreamSoundSource* source = new StreamSoundSource();
      source->set_sound_file(file.release());
      return source;
    }
  }
  
  SoundSource* source = new SoundSource();
  alSourcei(source->source, AL_BUFFER, buffer);
  return source;  
}

void
SoundManager::play(const std::string& filename, const Vector& pos)
{
  try {
    SoundSource* source = create_sound_source(filename);
    if(source == 0)
      return;
    if(pos == Vector(-1, -1)) {
      alSourcef(source->source, AL_ROLLOFF_FACTOR, 0);
    } else {
      source->set_position(pos);
    }
    source->play();
    sources.push_back(source);
  } catch(std::exception& e) {
    log_warning << "Couldn't play sound " << filename << ": " << e.what() << std::endl;
  }
}

void
SoundManager::enable_sound(bool enable)
{
  if(device == 0)
    return;
  sound_enabled = enable;
}

void
SoundManager::enable_music(bool enable)
{
  if(device == 0)
    return;
  music_enabled = enable;
  if(music_enabled) {
    play_music(current_music);
  } else {
    if(music_source) {
      delete music_source;
      music_source = 0;
    }
  }
}

void
SoundManager::stop_music(float fadetime)
{
  if(fadetime > 0) {
    if(music_source
        && music_source->get_fade_state() != StreamSoundSource::FadingOff)
      music_source->set_fading(StreamSoundSource::FadingOff, fadetime);
  } else {
    delete music_source;
    music_source = 0;
  }
  current_music = "";
}

void
SoundManager::play_music(const std::string& filename, bool fade)
{
  if(filename == current_music && music_source != NULL)
    return;
  current_music = filename;
  if(!music_enabled)
    return;

  if(filename == "") {
    delete music_source;
    music_source = 0;
    return;
  }

  try {
    std::auto_ptr<StreamSoundSource> newmusic (new StreamSoundSource());
    alSourcef(newmusic->source, AL_ROLLOFF_FACTOR, 0);
    newmusic->set_sound_file(load_sound_file(filename));
    newmusic->set_looping(true);
    if(fade)
      newmusic->set_fading(StreamSoundSource::FadingOn, .5f);
    newmusic->play();

    delete music_source;
    music_source = newmusic.release();
  } catch(std::exception& e) {
    log_warning << "Couldn't play music file '" << filename << "': " << e.what() << std::endl;
  }
}

void
SoundManager::set_listener_position(const Vector& pos)
{
  static Uint32 lastticks = 0;

  Uint32 current_ticks = SDL_GetTicks();
  if(current_ticks - lastticks < 300)
    return;
  lastticks = current_ticks;                 

  alListener3f(AL_POSITION, pos.x, pos.y, 0);
}

void
SoundManager::set_listener_velocity(const Vector& vel)
{
  alListener3f(AL_VELOCITY, vel.x, vel.y, 0);
}

void
SoundManager::update()
{
  static Uint32 lastticks = 0;

  Uint32 current_ticks = SDL_GetTicks();
  if(current_ticks - lastticks < 300)
    return;
  lastticks = current_ticks;

  // update and check for finished sound sources
  for(SoundSources::iterator i = sources.begin(); i != sources.end(); ) {
    SoundSource* source = *i;

    source->update();
    
    if(!source->playing()) {
      delete source;
      i = sources.erase(i);
    } else {
      ++i;
    }
  }
  // check streaming sounds
  if(music_source) {
    music_source->update();
  }
  
  alcProcessContext(context);
  check_alc_error("Error while processing audio context: ");
}

ALenum
SoundManager::get_sample_format(SoundFile* file)
{
  if(file->channels == 2) {
    if(file->bits_per_sample == 16) {
      return AL_FORMAT_STEREO16;
    } else if(file->bits_per_sample == 8) {
      return AL_FORMAT_STEREO8;
    } else {
      throw std::runtime_error("Only 16 and 8 bit samples supported");
    }
  } else if(file->channels == 1) {
    if(file->bits_per_sample == 16) {
      return AL_FORMAT_MONO16;
    } else if(file->bits_per_sample == 8) {
      return AL_FORMAT_MONO8;
    } else {
      throw std::runtime_error("Only 16 and 8 bit samples supported");
    }
  }
  
  throw std::runtime_error("Only 1 and 2 channel samples supported");
}

void
SoundManager::print_openal_version()
{
  log_info << "OpenAL Vendor: " << alGetString(AL_VENDOR) << std::endl;
  log_info << "OpenAL Version: " << alGetString(AL_VERSION) << std::endl;
  log_info << "OpenAL Renderer: " << alGetString(AL_RENDERER) << std::endl;
  log_info << "OpenAl Extensions: " << alGetString(AL_EXTENSIONS) << std::endl;
}

void
SoundManager::check_alc_error(const char* message)
{
  int err = alcGetError(device);
  if(err != ALC_NO_ERROR) {
    std::stringstream msg;
    msg << message << alcGetString(device, err);
    throw std::runtime_error(msg.str());
  }                
}

void
SoundManager::check_al_error(const char* message)
{
  int err = alGetError();
  if(err != AL_NO_ERROR) {
    std::stringstream msg;
    msg << message << alGetString(err);
    throw std::runtime_error(msg.str());
  }  
}

