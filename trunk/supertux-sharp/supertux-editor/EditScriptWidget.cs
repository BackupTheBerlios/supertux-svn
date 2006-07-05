using System;
using LispReader;
using Gtk;

/// <summary>
/// A widget used to open a dialog box to 
/// edit scripts in (and currently also other multi line strings).
/// </summary>
public sealed class EditScriptWidget : ICustomSettingsWidget
{
	public FieldOrProperty field;
	public FieldOrProperty Field {
		get {
			return field;
		}
		set {
			field = value;
		}
	}
	
	public object _object;
	public object Object {
		get {
			return _object;
		}
		set {
			_object = value;
		}
	}
	
	public Widget Create()
	{
		Button button = new Button("Edit Script");
		button.Clicked += OnEdit;
		button.Name = field.Name;
		
		return button;
	}
	
	private void OnEdit(object sender, EventArgs args)
	{
		new ScriptEditor(_object.GetType() + ":" + field.Name, field, Object);		
	}
}

[AttributeUsage(AttributeTargets.Field | AttributeTargets.Property,
                AllowMultiple=false)]
public sealed class EditScriptSetting : CustomSettingsWidgetAttribute
{
	public EditScriptSetting() : base(typeof(EditScriptWidget))
	{
	}
}
