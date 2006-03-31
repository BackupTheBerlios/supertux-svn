using System;
using System.IO;
using System.Reflection;
using Gtk;

public class ChooseResourceWidget : ICustomSettingsWidget
{
	public FieldInfo field;
	public FieldInfo Field {
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
	
	private Entry entry;
	
	public Widget Create()
	{
		HBox box = new HBox();
		entry = new Entry();
		entry.Text = field.GetValue(Object).ToString();
		box.Add(entry);
		
		Button chooseButton = new Button("...");
		box.Add(chooseButton);
		chooseButton.Clicked += OnChoose;
		
		box.Name = field.Name;
		return box;
	}
	
	private void OnChoose(object o, EventArgs args)
	{
		FileChooserDialog dialog = new FileChooserDialog("Choose resource", null, FileChooserAction.Open, new object[] {});
		dialog.AddButton(Gtk.Stock.Cancel, Gtk.ResponseType.Cancel);
		dialog.AddButton(Gtk.Stock.Open, Gtk.ResponseType.Ok);
		dialog.DefaultResponse = Gtk.ResponseType.Ok;
		
		dialog.Action = FileChooserAction.Open;
		dialog.SetFilename(Settings.Instance.SupertuxData + entry.Text);
		int result = dialog.Run();
		if(result != (int) ResponseType.Ok) {
			dialog.Destroy();
			return;
		}
		
		if(dialog.Filename.StartsWith(Settings.Instance.SupertuxData))
			entry.Text = dialog.Filename.Substring(Settings.Instance.SupertuxData.Length,
			                                          dialog.Filename.Length - Settings.Instance.SupertuxData.Length);
		else
			entry.Text = Path.GetFileName(dialog.Filename);
		
		dialog.Destroy();
	}
}
