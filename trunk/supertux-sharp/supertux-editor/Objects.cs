using Sprites;
using System;

[AttributeUsage(AttributeTargets.Class, AllowMultiple=false)]
public class SupertuxObjectAttribute : Attribute {
	public string Name;
	public string IconSprite;
	
	public SupertuxObjectAttribute(string Name, string IconSprite) {
		this.Name = Name;
		this.IconSprite = IconSprite;
	}
}

[SupertuxObject("mrbomb", "images/creatures/mr_bomb/mr_bomb.sprite")]
public class MrBomb : SimpleObject {
	public MrBomb() {
		Sprite = SpriteManager.Create("images/creatures/mr_bomb/mr_bomb.sprite");
		Sprite.Action = "left";
	}
}

[SupertuxObject("snowball", "images/creatures/snowball/snowball.sprite")]
public class Snowball : SimpleObject {
	public Snowball() {
		Sprite = SpriteManager.Create("images/creatures/snowball/snowball.sprite");
		Sprite.Action = "left";
	}
}

[SupertuxObject("stalactite", "images/creatures/stalactite/stalactite.sprite")]
public class Stalactite : SimpleObject {
	public Stalactite() {
		Sprite = SpriteManager.Create("images/creatures/stalactite/stalactite.sprite");
		Sprite.Action = "normal";
	}
}

[SupertuxObject("mriceblock", "images/creatures/mr_iceblock/mr_iceblock.sprite")]
public class MrIceBlock : SimpleObject {
	public MrIceBlock() {
		Sprite = SpriteManager.Create("images/creatures/mr_iceblock/mr_iceblock.sprite");
		Sprite.Action = "left";
	}
}

[SupertuxObject("bouncingsnowball", "images/creatures/bouncing_snowball/bouncing_snowball.sprite")]
public class BouncingSnowball : SimpleObject {
	public BouncingSnowball() {
		Sprite = SpriteManager.Create("images/creatures/bouncing_snowball/bouncing_snowball.sprite");
		Sprite.Action = "left";
	}
}

[SupertuxObject("flyingsnowball", "images/creatures/flying_snowball/flying_snowball.sprite")]
public class FlyingSnowball : SimpleObject {
	public FlyingSnowball() {
		Sprite = SpriteManager.Create("images/creatures/flying_snowball/flying_snowball.sprite");
		Sprite.Action = "left";
	}
}

[SupertuxObject("jumpy", "images/creatures/jumpy/jumpy.sprite")]
public class Jumpy : SimpleObject {
	public Jumpy() {
		Sprite = SpriteManager.Create("images/creatures/jumpy/jumpy.sprite");
		Sprite.Action = "left-up";
	}
}

[SupertuxObject("spiky", "images/creatures/spiky/spiky.sprite")]
public class Spiky : SimpleObject {
	public Spiky() {
		Sprite = SpriteManager.Create("images/creatures/spiky/spiky.sprite");
		Sprite.Action = "left";
	}
}

[SupertuxObject("spawnpoint", "images/engine/editor/spawnpoint.png")]
public class SpawnPoint : SimpleObject {
	[LispChild("name")]
	public string Name;
	
	public SpawnPoint() {
		Sprite = SpriteManager.CreateFromImage("images/engine/editor/spawnpoint.png");
		Sprite.Action = "default";
	}
}

[SupertuxObject("flame", "images/creatures/flame/flame.sprite")]
public class Flame : SimpleObject {
	public Flame() {
		Sprite = SpriteManager.Create("images/creatures/flame/flame.sprite");
		Sprite.Action = "normal";
	}
}

[SupertuxObject("fish", "images/creatures/fish/fish.sprite")]
public class Fish : SimpleObject {
	public Fish() {
		Sprite = SpriteManager.Create("images/creatures/fish/fish.sprite");
		Sprite.Action = "normal";
	}
}

[SupertuxObject("mrtree", "images/creatures/mr_tree/mr_tree.sprite")]
public class MrTree : SimpleObject {
	public MrTree() {
		Sprite = SpriteManager.Create("images/creatures/mr_tree/mr_tree.sprite");
		Sprite.Action = "large-left";
	}
}

[SupertuxObject("poisonivy", "images/creatures/poison_ivy/poison_ivy.sprite")]
public class PoisonIvy : SimpleObject {
	public PoisonIvy() {
		Sprite = SpriteManager.Create("images/creatures/poison_ivy/poison_ivy.sprite");
		Sprite.Action = "left";
	}
}

[SupertuxObject("zeekling", "images/creatures/zeekling/zeekling.sprite")]
public class Zeekling : SimpleObject {
	public Zeekling() {
		Sprite = SpriteManager.Create("images/creatures/zeekling/zeekling.sprite");
		Sprite.Action = "left";
	}
}

[SupertuxObject("kugelblitz", "images/creatures/kugelblitz/kugelblitz.sprite")]
public class Kugelblitz : SimpleObject {
	public Kugelblitz() {
		Sprite = SpriteManager.Create("images/creatures/kugelblitz/kugelblitz.sprite");
		Sprite.Action = "falling";
	}
}

[SupertuxObject("dispenser", "images/creatures/dispenser/dispenser.sprite")]
public class Dispenser : SimpleObject {
	public Dispenser() {
		Sprite = SpriteManager.Create("images/creatures/dispenser/dispenser.sprite");
		Sprite.Action = "working-left";
	}
}

[SupertuxObject("yeti", "images/creatures/yeti/yeti.sprite")]
public class Yeti : SimpleObject {
	public Yeti() {
		Sprite = SpriteManager.Create("images/creatures/yeti/yeti.sprite");
		Sprite.Action = "left";
	}
}

[SupertuxObject("stalactite_yeti", "images/engine/editor/stalactite_yeti.png")]
public class StalactiteYeti : SimpleObject {
	public StalactiteYeti() {
		Sprite = SpriteManager.Create("images/creatures/stalactite/stalactite.sprite");
		Sprite.Action = "normal";
	}
}

[SupertuxObject("door", "images/objects/door/door.sprite")]
public class Door : SimpleObject {
	[LispChild("sector")]
	public string Sector;
	[LispChild("spawnpoint")]
	public string Spawnpoint;
	
	public Door() {
		Sprite = SpriteManager.Create("images/objects/door/door.sprite");
		Sprite.Action = "normal";
	}
}

[SupertuxObject("hatch", "images/objects/hatch/hatch.sprite")]
public class Hatch : SimpleObject {
	[LispChild("sector")]
	public string Sector;
	[LispChild("spawnpoint")]
	public string Spawnpoint;
	
 	public Hatch() {
		Sprite = SpriteManager.Create("images/objects/hatch/hatch.sprite");
		Sprite.Action = "normal";
	}
}

[SupertuxObject("trampoline", "images/objects/trampoline/trampoline.sprite")]
public class Trampoline : SimpleObject {
	public Trampoline() {
		Sprite = SpriteManager.Create("images/objects/trampoline/trampoline.sprite");
		Sprite.Action = "normal";
	}
}

[SupertuxObject("bell", "images/objects/bell/bell.sprite")]
public class Bell : SimpleObject {
	public Bell() {
		Sprite = SpriteManager.Create("images/objects/bell/bell.sprite");
		Sprite.Action = "normal";
	}
}

[SupertuxObject("rock", "images/objects/rock/rock.sprite")]
public class Rock : SimpleObject {
	public Rock() {
		Sprite = SpriteManager.Create("images/objects/rock/rock.sprite");
		Sprite.Action = "normal";
	}
}

[SupertuxObject("unstable_tile", "images/objects/unstable_tile/unstable_tile.sprite")]
public class UnstableTile : SimpleObject {
	public UnstableTile() {
		Sprite = SpriteManager.Create("images/objects/unstable_tile/unstable_tile.sprite");
		Sprite.Action = "normal";
	}
}

[SupertuxObject("infoblock", "images/engine/editor/infoblock.png")]
public class InfoBlock : SimpleObject {
	public InfoBlock() {
		//Sprite = SpriteManager.Create("images/objects/
		// ...
	}
}

[SupertuxObject("powerup", "images/engine/editor/powerup.png")]
public class Powerup : SimpleObject {
	// TODO contents...
	
	public Powerup() {
	}
}

[SupertuxObject("scriptedobject", "images/engine/editor/scriptedobject.png")]
public class ScriptedObject : SimpleObject {
	[LispChild("name")]
	public string Name;
	[ChooseResourceSetting]	
	[LispChild("sprite")]
	public string SpriteFile {
		get {
			return spriteFile;
		}
		set {
			Sprite = SpriteManager.Create(value);
			spriteFile = value;
		}
	}
	private string spriteFile = "";
	[LispChild("layer")]
	public int Layer;
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

[SupertuxObject("ambient_sound", "images/engine/editor/ambientsound.png")]
public class AmbientSound : SimpleObjectArea {
	[LispChild("sample")]
	[ChooseResourceSetting]	
	public string Sample;
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
public class SequenceTrigger : SimpleObjectArea {
	[LispChild("sequence")]
	public string Sequence;

	public SequenceTrigger() {
		Color = new Drawing.Color(.8f, 0, 0, 0.8f);
	}
}

[SupertuxObject("secretarea",  "images/engine/editor/secretarea.png")]
public class SecretArea : SimpleObjectArea {
	[LispChild("message")]
	public string Message;
	
	public SecretArea() {
		Color = new Drawing.Color(0, .8f, 0, 0.8f);
	}
}
