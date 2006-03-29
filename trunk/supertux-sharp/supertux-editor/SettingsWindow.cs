using System;
using System.IO;
using System.Collections;
using System.Collections.Generic;
using System.Reflection;
using Gtk;

public class SettingsWindow
{
	private Window window;
	private System.Object Object;
	private Hashtable fieldTable = new Hashtable();
	
	public SettingsWindow(string Title, System.Object Object)
	{
		this.Object = Object;
		try {
			CreateWindow(Title);
		} catch(Exception e) {
			ErrorDialog.Exception(e);
		}
	}
	
	private object CreateObject(Type Type) {
		// create object
		ConstructorInfo Constructor = Type.GetConstructor(Type.EmptyTypes);
		if(Constructor == null)
			throw new Exception("Type '" + Type + "' has no public constructor without arguments");
		object Result = Constructor.Invoke(new object[] {});

		return Result;
	}
	
	private void CreateWindow(string Title)
	{
		window = new Window(Title);
		
		VBox box = new VBox();
		
		// iterate over all fields and properties
		Type Type = Object.GetType();
		FieldInfo[] fields = Type.GetFields();
		List<Widget> editWidgets = new List<Widget>();
		foreach(FieldInfo field in fields) {
			CustomSettingsWidgetAttribute customSettings = (CustomSettingsWidgetAttribute)
			Attribute.GetCustomAttribute(field, typeof(CustomSettingsWidgetAttribute));
			if(customSettings != null) {
				Type type = customSettings.Type;
				ICustomSettingsWidget customWidget = (ICustomSettingsWidget) CreateObject(type);
				customWidget.Object = Object;
				customWidget.Field = field;
				editWidgets.Add(customWidget.Create());
				continue;				
			}
			
			LispChildAttribute ChildAttrib = (LispChildAttribute)
				Attribute.GetCustomAttribute(field, typeof(LispChildAttribute));
			if(ChildAttrib == null)
				continue;
			
			if(field.FieldType == typeof(string) || field.FieldType == typeof(float)
				|| field.FieldType == typeof(int)) {
				Entry entry = new Entry();
				entry.Name = field.Name;
				entry.Text = field.GetValue(Object).ToString();
				fieldTable[field.Name] = field;
				entry.Changed += OnEntryChanged;
				editWidgets.Add(entry);
			} else if(field.FieldType == typeof(bool)) {
				CheckButton checkButton = new CheckButton(field.Name);
				checkButton.Name = field.Name;
				checkButton.Active = (bool) field.GetValue(Object);
				fieldTable[field.Name] = field;
				checkButton.Toggled += OnCheckButtonToggled;
				editWidgets.Add(checkButton);
			}
			
		}
		
		Table table = new Table((uint) editWidgets.Count, 2, false);
		table.ColumnSpacing = 6;
		table.RowSpacing = 6;
		table.BorderWidth = 12;
		for(uint i = 0; i < editWidgets.Count; ++i) {
			Widget widget = editWidgets[(int) i];
			if(widget is CheckButton) {
				table.Attach(widget, 0, 2, i, i+1,
				                 AttachOptions.Fill | AttachOptions.Expand, AttachOptions.Shrink, 0, 0);
			} else {
				Label label = new Label(widget.Name + ":");
				label.Layout.Alignment = Pango.Alignment.Left;
				table.Attach(label, 0, 1, i, i+1,
			    	            AttachOptions.Fill, AttachOptions.Shrink, 0, 0);
				table.Attach(widget, 1, 2, i, i+1,
				                 AttachOptions.Fill | AttachOptions.Expand, AttachOptions.Shrink, 0, 0);
			}
		}
		box.PackStart(table, true, true, 0);
		
		ButtonBox buttonBox = new HButtonBox();
		buttonBox.BorderWidth = 12;
		buttonBox.Spacing = 6;
		
		Button closeButton = new Button(Stock.Close);
		closeButton.Clicked += OnCloseClicked;
		buttonBox.Add(closeButton);
		buttonBox.Layout = ButtonBoxStyle.End;
		
		box.PackStart(buttonBox, false, false, 0);
		
		window.Add(box);
		window.Resizable = true;
		window.DefaultWidth = 500;
		window.ShowAll();
	}
	
	private void OnCloseClicked(object o, EventArgs args)
	{
		window.Hide();	
	}
	
	private void OnEntryChanged(object o, EventArgs args)
	{
		try {
			Entry entry = (Entry) o;
			FieldInfo field = (FieldInfo) fieldTable[entry.Name];
			if(field.FieldType == typeof(string)) {
				field.SetValue(Object, entry.Text);
			} else if(field.FieldType == typeof(float)) {
				float parsed = Single.Parse(entry.Text);
				if(parsed.ToString() != entry.Text)
					entry.Text = parsed.ToString();
				field.SetValue(Object, parsed);
			}
		} catch(Exception e) {
			ErrorDialog.Exception(e);
		}
	}
	
	private void OnCheckButtonToggled(object o, EventArgs args)
	{
		try {
			CheckButton checkButton = (CheckButton) o;
			FieldInfo field = (FieldInfo) fieldTable[checkButton.Name];
			field.SetValue(Object, checkButton.Active);
		} catch(Exception e) {
			ErrorDialog.Exception(e);
		}			
	}	
}
