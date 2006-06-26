using Sprites;
using System;
using LispReader;
using DataStructures;
using SceneGraph;

/// <summary>Tells where the object can be used.</summary>
public enum SupertuxObjectUsage {
	/// <summary>Can be used anywhere. This is the default.</summary>
	any,
	/// <summary>Can only be used on worldmaps.</summary>
	worldmapOnly,
	/// <summary>Can only be used in "normal" levels.</summary>
	levelOnly
}

// TODO: Write better documentation.
/// <summary>Attribute that marks a class as an object for use in levels/worldmaps.</summary>
[AttributeUsage(AttributeTargets.Class, AllowMultiple=false)]
public class SupertuxObjectAttribute : Attribute
{
	public string Name;
	public string IconSprite;
	/// <summary>A <see cref="SupertuxObjectUsage"/> describing where the object can be used.</summary>
	public SupertuxObjectUsage Target;

	/// <summary>Constructs for <see cref="SupertuxObjectAttribute"/>.</summary>
	/// <param name="Name">Name of object in the level file</param>
	/// <param name="IconSprite">Icon used in the object list in the editor</param>
	public SupertuxObjectAttribute(string Name, string IconSprite) {
		this.Name = Name;
		this.IconSprite = IconSprite;
	}
}

[SupertuxObject("mrbomb", "images/creatures/mr_bomb/mr_bomb.sprite", Target = SupertuxObjectUsage.levelOnly)]
public class MrBomb : SimpleObject
{
	public MrBomb() {
		Sprite = SpriteManager.Create("images/creatures/mr_bomb/mr_bomb.sprite");
		Sprite.Action = "left";
	}
}

[SupertuxObject("snowball", "images/creatures/snowball/snowball.sprite", Target = SupertuxObjectUsage.levelOnly)]
public class Snowball : SimpleObject
{
	public Snowball() {
		Sprite = SpriteManager.Create("images/creatures/snowball/snowball.sprite");
		Sprite.Action = "left";
	}
}

[SupertuxObject("stalactite", "images/creatures/stalactite/stalactite.sprite", Target = SupertuxObjectUsage.levelOnly)]
public class Stalactite : SimpleObject
{
	public Stalactite() {
		Sprite = SpriteManager.Create("images/creatures/stalactite/stalactite.sprite");
		Sprite.Action = "normal";
	}
}

[SupertuxObject("mriceblock", "images/creatures/mr_iceblock/mr_iceblock.sprite", Target = SupertuxObjectUsage.levelOnly)]
public class MrIceBlock : SimpleObject
{
	public MrIceBlock() {
		Sprite = SpriteManager.Create("images/creatures/mr_iceblock/mr_iceblock.sprite");
		Sprite.Action = "left";
	}
}

[SupertuxObject("bouncingsnowball", "images/creatures/bouncing_snowball/bouncing_snowball.sprite", Target = SupertuxObjectUsage.levelOnly)]
public class BouncingSnowball : SimpleObject
{
	public BouncingSnowball() {
		Sprite = SpriteManager.Create("images/creatures/bouncing_snowball/bouncing_snowball.sprite");
		Sprite.Action = "left";
	}
}

[SupertuxObject("flyingsnowball", "images/creatures/flying_snowball/flying_snowball.sprite", Target = SupertuxObjectUsage.levelOnly)]
public class FlyingSnowball : SimpleObject
{
	public FlyingSnowball() {
		Sprite = SpriteManager.Create("images/creatures/flying_snowball/flying_snowball.sprite");
		Sprite.Action = "left";
	}
}

[SupertuxObject("jumpy", "images/creatures/jumpy/jumpy.sprite", Target = SupertuxObjectUsage.levelOnly)]
public class Jumpy : SimpleObject
{
	public Jumpy() {
		Sprite = SpriteManager.Create("images/creatures/jumpy/jumpy.sprite");
		Sprite.Action = "left-up";
	}
}

[SupertuxObject("spiky", "images/creatures/spiky/spiky.sprite", Target = SupertuxObjectUsage.levelOnly)]
public class Spiky : SimpleObject
{
	public Spiky() {
		Sprite = SpriteManager.Create("images/creatures/spiky/spiky.sprite");
		Sprite.Action = "left";
	}
}

[SupertuxObject("sspiky", "images/creatures/spiky/sleepingspiky.sprite", Target = SupertuxObjectUsage.levelOnly)]
public class SleepSpiky : SimpleObject
{
	public SleepSpiky() {
		Sprite = SpriteManager.Create("images/creatures/spiky/sleepingspiky.sprite");
		Sprite.Action = "sleeping-left";
	}
}

[SupertuxObject("spawnpoint", "images/engine/editor/spawnpoint.png", Target = SupertuxObjectUsage.levelOnly)]
public class SpawnPoint : SimpleObject
{
	[LispChild("name")]
	public string Name;
	
	public SpawnPoint() {
		Sprite = SpriteManager.CreateFromImage("images/engine/editor/spawnpoint.png");
		Sprite.Action = "default";
	}
}

[SupertuxObject("flame", "images/creatures/flame/flame.sprite", Target = SupertuxObjectUsage.levelOnly)]
public class Flame : SimpleObject
{
	[LispChild("radius", Optional = true, Default = 100f)]
	public float Radius = 100f;
	[LispChild("speed", Optional = true, Default = 2f)]
	public float Speed = 2;

	public Flame() {
		Sprite = SpriteManager.Create("images/creatures/flame/flame.sprite");
		Sprite.Action = "normal";
	}
}

[SupertuxObject("fish", "images/creatures/fish/fish.sprite", Target = SupertuxObjectUsage.levelOnly)]
public class Fish : SimpleObject
{
	public Fish() {
		Sprite = SpriteManager.Create("images/creatures/fish/fish.sprite");
		Sprite.Action = "normal";
	}
}

[SupertuxObject("mrtree", "images/creatures/mr_tree/mr_tree.sprite", Target = SupertuxObjectUsage.levelOnly)]
public class MrTree : SimpleObject
{
	public MrTree() {
		Sprite = SpriteManager.Create("images/creatures/mr_tree/mr_tree.sprite");
		Sprite.Action = "large-left";
	}
}

[SupertuxObject("poisonivy", "images/creatures/poison_ivy/poison_ivy.sprite", Target = SupertuxObjectUsage.levelOnly)]
public class PoisonIvy : SimpleObject
{
	public PoisonIvy() {
		Sprite = SpriteManager.Create("images/creatures/poison_ivy/poison_ivy.sprite");
		Sprite.Action = "left";
	}
}

[SupertuxObject("zeekling", "images/creatures/zeekling/zeekling.sprite", Target = SupertuxObjectUsage.levelOnly)]
public class Zeekling : SimpleObject
{
	public Zeekling() {
		Sprite = SpriteManager.Create("images/creatures/zeekling/zeekling.sprite");
		Sprite.Action = "left";
	}
}

[SupertuxObject("snail", "images/creatures/snail/snail.sprite", Target = SupertuxObjectUsage.levelOnly)]
public class Snail : SimpleObject
{
	public Snail() {
		Sprite = SpriteManager.Create("images/creatures/snail/snail.sprite");
		Sprite.Action = "left";
	}
}

[SupertuxObject("totem", "images/creatures/totem/totem.sprite", Target = SupertuxObjectUsage.levelOnly)]
public class Totem : SimpleObject
{
	public Totem() {
		Sprite = SpriteManager.Create("images/creatures/totem/totem.sprite");
		Sprite.Action = "walking-left";
	}
}

[SupertuxObject("kugelblitz", "images/creatures/kugelblitz/kugelblitz.sprite", Target = SupertuxObjectUsage.levelOnly)]
public class Kugelblitz : SimpleObject
{
	public Kugelblitz() {
		Sprite = SpriteManager.Create("images/creatures/kugelblitz/kugelblitz.sprite");
		Sprite.Action = "falling";
	}
}

[SupertuxObject("dispenser", "images/creatures/dispenser/dispenser.sprite", Target = SupertuxObjectUsage.levelOnly)]
public class Dispenser : SimpleObject
{
	[LispChild("badguy")]
	public string Badguy = "";
	[LispChild("cycle")]
	public float Cycle = 1;

	public Dispenser() {
		Sprite = SpriteManager.Create("images/creatures/dispenser/dispenser.sprite");
		Sprite.Action = "working-left";
	}
}

[SupertuxObject("yeti", "images/creatures/yeti/yeti.sprite", Target = SupertuxObjectUsage.levelOnly)]
public class Yeti : SimpleObject
{
	[LispChild("dead-script")]
	[EditScriptSetting]
	public String DeadScript = "";

	public Yeti() {
		Sprite = SpriteManager.Create("images/creatures/yeti/yeti.sprite");
		Sprite.Action = "left";
	}
}

[SupertuxObject("stalactite_yeti", "images/engine/editor/stalactite_yeti.png", Target = SupertuxObjectUsage.levelOnly)]
public class StalactiteYeti : SimpleObject
{
	public StalactiteYeti() {
		Sprite = SpriteManager.Create("images/creatures/stalactite/stalactite.sprite");
		Sprite.Action = "normal";
	}
}

[SupertuxObject("door", "images/objects/door/door.sprite", Target = SupertuxObjectUsage.levelOnly)]
public class Door : SimpleObject
{
	[LispChild("sector")]
	public string Sector;
	[LispChild("spawnpoint")]
	public string Spawnpoint;
	
	public Door() {
		Sprite = SpriteManager.Create("images/objects/door/door.sprite");
		Sprite.Action = "normal";
	}
}

[SupertuxObject("hatch", "images/objects/hatch/hatch.sprite", Target = SupertuxObjectUsage.levelOnly)]
public class Hatch : SimpleObject
{
	[LispChild("sector")]
	public string Sector = "";
	[LispChild("spawnpoint")]
	public string Spawnpoint = "";
	
 	public Hatch() {
		Sprite = SpriteManager.Create("images/objects/hatch/hatch.sprite");
		Sprite.Action = "normal";
	}
}

[SupertuxObject("switch", "images/objects/switch/switch.sprite", Target = SupertuxObjectUsage.levelOnly)]
public class Switch : SimpleObject
{
	[ChooseResourceSetting]	
	[LispChild("sprite")]
	public string SpriteFile {
		get {
			return spriteFile;
		}
		set {
			spriteFile = value;
			if(value != "") {
				Sprite = SpriteManager.Create(value);
				Sprite.Action = "off";
			}
		}
	}
	private string spriteFile = "";

	[LispChild("script")]
	[EditScriptSetting]
	public string Script = "";

	public Switch() {
		Sprite = SpriteManager.Create("images/objects/switch/switch.sprite");
		Sprite.Action = "off";
	}
}

[SupertuxObject("trampoline", "images/objects/trampoline/trampoline.sprite", Target = SupertuxObjectUsage.levelOnly)]
public class Trampoline : SimpleObject
{
	public Trampoline() {
		Sprite = SpriteManager.Create("images/objects/trampoline/trampoline.sprite");
		Sprite.Action = "normal";
	}
}

[SupertuxObject("firefly", "images/objects/firefly/firefly.sprite", Target = SupertuxObjectUsage.levelOnly)]
public class Firefly : SimpleObject
{
	public Firefly() {
		Sprite = SpriteManager.Create("images/objects/firefly/firefly.sprite");
		Sprite.Action = "normal";
	}
}

[SupertuxObject("angrystone", "images/creatures/angrystone/angrystone.sprite", Target = SupertuxObjectUsage.levelOnly)]
public class AngryStone : SimpleObject
{
	public AngryStone() {
		Sprite = SpriteManager.Create("images/creatures/angrystone/angrystone.sprite");
	}
}

/// <summary>Base class for platforms.</summary>
public abstract class PlatformBase : IGameObject, IObject, IPathObject, Node
{
	[LispChild("name", Optional = true, Default = "")]
	public string Name = "";
	[LispChild("running", Optional = true, Default = true)]
	public bool Running = true;
	[ChooseResourceSetting]	
	[LispChild("sprite")]
	public string SpriteFile {
		get {
			return spriteFile;
		}
		set {
			spriteFile = value;
			if(value != "")
				Sprite = SpriteManager.Create(value);
		}
	}
	private string spriteFile = "";
	
	private Sprite Sprite;
	
	private Path path;
	[LispChild("path")]
	public Path Path {
		get {
			return path;
		}
		set {
			path = value;
		}
	}
	
	public virtual bool Resizable {
		get {
			return false;
		}
	}

	public PlatformBase()
	{
		Sprite = SpriteManager.Create("images/objects/flying_platform/flying_platform.sprite");
		path = new Path();
		path.Nodes.Add(new Path.Node());
	}
	
	public void Draw()
	{
		Sprite.Draw(Path.Nodes[0].Pos);
	}
	
	public virtual Node GetSceneGraphNode() {
		return this;
	}
	
	public virtual void ChangeArea(RectangleF NewArea) {
	}
	
	public virtual RectangleF Area {
		get {
			float x = Path.Nodes[0].X;
			float y = Path.Nodes[0].Y;
			
			return new RectangleF(x - Sprite.Offset.X, y - Sprite.Offset.Y, Sprite.Width, Sprite.Height);
		}
	}	
}

[SupertuxObject("platform", "images/objects/flying_platform/flying_platform.sprite", Target = SupertuxObjectUsage.levelOnly)]
public class FlyingPlatform : PlatformBase
{
}

[SupertuxObject("hurting_platform", "images/objects/sawblade/sawblade.sprite", Target = SupertuxObjectUsage.levelOnly)]
public class HurtingPlatform : PlatformBase
{
}

[SupertuxObject("willowisp", "images/creatures/willowisp/willowisp.sprite", Target = SupertuxObjectUsage.levelOnly)]
public class WilloWisp : SimpleObject
{
	[LispChild("sector")]
	public string Sector = "";
	[LispChild("spawnpoint")]
	public string SpawnPoint = "";
	
	public WilloWisp() {
		Sprite = SpriteManager.Create("images/creatures/willowisp/willowisp.sprite");
		Sprite.Action = "idle";
	}
}

[SupertuxObject("darttrap", "images/creatures/darttrap/darttrap.sprite", Target = SupertuxObjectUsage.levelOnly)]
public class DartTrap : SimpleObject
{
	[LispChild("initial-delay")]
	public float initial_delay = 0;
	[LispChild("fire-delay")]
	public float fire_delay = 2;
	[LispChild("ammo")]
	public int ammo = -1;
	
	public DartTrap() {
		Sprite = SpriteManager.Create("images/creatures/darttrap/darttrap.sprite");
		Sprite.Action = "idle-left";
	}
}

[SupertuxObject("skullyhop", "images/creatures/skullyhop/skullyhop.sprite", Target = SupertuxObjectUsage.levelOnly)]
public class SkullyHop : SimpleObject
{
	public SkullyHop() {
		Sprite = SpriteManager.Create("images/creatures/skullyhop/skullyhop.sprite");
		Sprite.Action = "standing-left";
	}
}

[SupertuxObject("igel", "images/creatures/igel/igel.sprite", Target = SupertuxObjectUsage.levelOnly)]
public class Igel : SimpleObject
{
	public Igel() {
		Sprite = SpriteManager.Create("images/creatures/igel/igel.sprite");
		Sprite.Action = "walking-left";
	}
}

[SupertuxObject("rock", "images/objects/rock/rock.sprite", Target = SupertuxObjectUsage.levelOnly)]
public class Rock : SimpleObject
{
	public Rock() {
		Sprite = SpriteManager.Create("images/objects/rock/rock.sprite");
		Sprite.Action = "normal";
	}
}

[SupertuxObject("candle", "images/objects/candle/candle.sprite", Target = SupertuxObjectUsage.levelOnly)]
public class Candle : SimpleObject
{
	[LispChild("name", Optional = true, Default = "")]
	public string Name = "";
	[LispChild("burning", Optional = true, Default = true)]
	public bool Burning = true;

	public Candle() {
		Sprite = SpriteManager.Create("images/objects/candle/candle.sprite");
		Sprite.Action = "on";
	}
}

[SupertuxObject("unstable_tile", "images/objects/unstable_tile/unstable_tile.sprite", Target = SupertuxObjectUsage.levelOnly)]
public class UnstableTile : SimpleObject
{
	[ChooseResourceSetting]	
	[LispChild("sprite")]
	public string SpriteFile {
		get {
			return spriteFile;
		}
		set {
			spriteFile = value;
			if(value != "") {
				Sprite = SpriteManager.Create(value);
				Sprite.Action = "normal";
			}
		}
	}
	private string spriteFile = "";

	public UnstableTile() {
		Sprite = SpriteManager.Create("images/objects/unstable_tile/unstable_tile.sprite");
		Sprite.Action = "normal";
	}
}

[SupertuxObject("weak_block", "images/objects/strawbox/strawbox.sprite", Target = SupertuxObjectUsage.levelOnly)]
public class WeakBlock : SimpleObject
{
	public WeakBlock() {
		Sprite = SpriteManager.Create("images/objects/strawbox/strawbox.sprite");
		Sprite.Action = "normal";
	}
}

[SupertuxObject("infoblock", "images/objects/bonus_block/infoblock.sprite", Target = SupertuxObjectUsage.levelOnly)]
public class InfoBlock : SimpleObject
{
	[LispChild("message")]
	[EditScriptSetting]
	public string Message = "";

	public InfoBlock() {
		Sprite = SpriteManager.Create("images/objects/bonus_block/infoblock.sprite");
	}
}

[SupertuxObject("powerup", "images/engine/editor/powerup.png", Target = SupertuxObjectUsage.levelOnly)]
public class Powerup : SimpleObject
{
	[ChooseResourceSetting]	
	[LispChild("sprite")]
	public string SpriteFile {
		get {
			return spriteFile;
		}
		set {
			spriteFile = value;
			try {
				if(value != "")
					Sprite = SpriteManager.Create(value);
			} catch(Exception e) {
				ErrorDialog.Exception(e);
			}
		}
	}
	private string spriteFile = "";
	[LispChild("script", Optional = true, Default = "")]
	[EditScriptSetting]	
	public string Script = "";
	[LispChild("disable-physics", Optional = true, Default = false)]
	public bool DisablePhysics;
	
	public Powerup() {
	}
}

[SupertuxObject("scriptedobject", "images/engine/editor/scriptedobject.png")]
public class ScriptedObject : SimpleObject
{
	[LispChild("name")]
	public string Name = "";
	[ChooseResourceSetting]	
	[LispChild("sprite")]
	public string SpriteFile {
		get {
			return spriteFile;
		}
		set {
			spriteFile = value;
			if(value != "")
				Sprite = SpriteManager.Create(value);
		}
	}
	private string spriteFile = "";
	[LispChild("z-pos", Optional = true, Default = -10)]
	public int ZPos = -10;
	[LispChild("visible")]
	public bool Visible = true;
	[LispChild("physic-enabled")]
	public bool PhysicEnabled = false;
	[LispChild("solid")]
	public bool Solid = false;
	
	public ScriptedObject() {
		Sprite = SpriteManager.CreateFromImage("images/engine/editor/scriptedobject.png");
		Sprite.Action = "default";
	}	
}

[SupertuxObject("wind", "images/engine/editor/wind.png", Target = SupertuxObjectUsage.levelOnly)]
public class Wind : SimpleObjectArea
{
	[LispChild("speed-x")]
	public float SpeedX = 0;

	[LispChild("speed-y")]
	public float SpeedY = 0;

	[LispChild("acceleration")]
	public float Acceleration = 0;

	public Wind() {
		Color = new Drawing.Color(.8f, 0, 0.8f, 0.8f);
	}
}

[SupertuxObject("ambient_sound", "images/engine/editor/ambientsound.png")]
public class AmbientSound : SimpleObjectArea
{
	[LispChild("sample")]
	[ChooseResourceSetting]	
	public string Sample = "";
	[LispChild("distance_factor")]
	public float DistanceFactor;
	[LispChild("distance_bias")]
	public float DistanceBias;
	[LispChild("volume")]
	public float Volume;
	
	public AmbientSound() {
		Sprite = SpriteManager.CreateFromImage("images/engine/editor/ambientsound.png");
		Sprite.Action = "default";
		Color = new Drawing.Color(0, 0, 0.8f, 0.8f);
	}
}

[SupertuxObject("sequencetrigger", "images/engine/editor/sequencetrigger.png")]
public class SequenceTrigger : SimpleObjectArea
{
	[LispChild("sequence")]
	public string Sequence = "";

	public SequenceTrigger() {
		Color = new Drawing.Color(.8f, 0, 0, 0.8f);
	}
}

[SupertuxObject("scripttrigger", "images/engine/editor/scripttrigger.png")]
public class ScriptTrigger : SimpleObjectArea
{
	[LispChild("script")]
	[EditScriptSetting]
	public string Script = "";
	[LispChild("button")]
	public bool IsButton;

	public ScriptTrigger() {
		Color = new Drawing.Color(.8f, 0, .8f, 0.8f);
	}
}

[SupertuxObject("secretarea",  "images/engine/editor/secretarea.png")]
public class SecretArea : SimpleObjectArea
{
	public SecretArea() {
		Color = new Drawing.Color(0, .8f, 0, 0.8f);
	}
}

[SupertuxObject("particles-rain", "images/engine/editor/rain.png")]
public class RainParticles : IGameObject
{
	[LispChild("z-pos", Optional = true, Default = 0)]
	public int ZPos = 0;
}

[SupertuxObject("particles-ghosts", "images/engine/editor/ghostparticles.png")]
public class GhostParticles : IGameObject
{
	[LispChild("z-pos", Optional = true, Default = -200)]
	public int ZPos = -200;
}

[SupertuxObject("particles-snow", "images/engine/editor/snow.png")]
public class SnowParticles : IGameObject
{
	[LispChild("z-pos", Optional = true, Default = -200)]
	public int ZPos = -200;
}

[SupertuxObject("particles-clouds", "images/engine/editor/clouds.png")]
public class CloudParticles : IGameObject
{
	[LispChild("z-pos", Optional = true, Default = -200)]
	public int ZPos = -200;
}

[SupertuxObject("leveltime", "images/engine/editor/clock.png", Target = SupertuxObjectUsage.levelOnly)]
public class LevelTime : IGameObject
{
	[LispChild("time")]
	public float Time;
}
