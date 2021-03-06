//  $Id$
using DataStructures;
using OpenGl;
using System;
using Gdk;

public sealed class ReplaceEditor : TileEditorBase, IEditor, IDisposable {

	public event RedrawEventHandler Redraw;

	public ReplaceEditor(IEditorApplication application, Tilemap Tilemap, Tileset Tileset, Selection selection)
	{
		this.application = application;
		this.Tilemap = Tilemap;
		this.Tileset = Tileset;
		this.selection = selection;
		application.TilemapChanged += OnTilemapChanged;
		selection.Changed += OnSelectionChanged;
	}

	public void OnTilemapChanged(Tilemap newTilemap)
	{
		Tilemap = newTilemap;
	}

	private void Replace(int oldId, int newId) {
		for (int x = 0; x < Tilemap.Width; x++) {
			for (int y = 0; y < Tilemap.Height; y++) {
				if (Tilemap[x,y] == oldId) Tilemap[x,y] = newId;
			}
		}
	}

	public void Dispose()
	{
		selection.Changed -= OnSelectionChanged;
	}

	public void OnMouseButtonPress(Vector MousePos, int button, ModifierType Modifiers)
	{
		if (Tilemap == null) return;

		UpdateMouseTilePos(MousePos);

		if(button == 1) {
			if ((selection.Width == 1) && (selection.Height == 1)) {
				application.TakeUndoSnapshot("Replace Tool");
				Replace(Tilemap[MouseTilePos], selection[0,0]);
			}
			LastDrawPos = MouseTilePos;
			drawing = true;
			Redraw();
		}
		if(button == 3) {
			if(MouseTilePos.X < 0 || MouseTilePos.Y < 0
			   || MouseTilePos.X >= Tilemap.Width
			   || MouseTilePos.Y >= Tilemap.Height)
				return;

			SelectStartPos = MouseTilePos;
			selecting = true;
			UpdateSelection();
			Redraw();
		}
	}

	public void OnMouseButtonRelease(Vector MousePos, int button, ModifierType Modifiers)
	{
		if (Tilemap == null) return;

		UpdateMouseTilePos(MousePos);

		if(button == 1) {
			drawing = false;
		}
		if(button == 3) {
			UpdateSelection();

			uint NewWidth = (uint) (SelectionP2.X - SelectionP1.X) + 1;
			uint NewHeight = (uint) (SelectionP2.Y - SelectionP1.Y) + 1;
			selection.Resize(NewWidth, NewHeight, 0);
			for(uint y = 0; y < NewHeight; y++) {
				for(uint x = 0; x < NewWidth; ++x) {
					selection[x, y]
						= Tilemap[(uint) SelectionP1.X + x,
						          (uint) SelectionP1.Y + y];
				}
			}

			selection.FireChangedEvent();
			selecting = false;
		}

		Redraw();
	}

	public void OnMouseMotion(Vector MousePos, ModifierType Modifiers)
	{
		if (Tilemap == null) return;

		if(UpdateMouseTilePos(MousePos)) {
			if(selection.Width == 0 || selection.Height == 0)
				return;

			if(drawing &&
			   ( (Modifiers & ModifierType.ShiftMask) != 0 ||
			     ((LastDrawPos.X - MouseTilePos.X) % selection.Width == 0 &&
			      (LastDrawPos.Y - MouseTilePos.Y) % selection.Height == 0
			     )
			   )
			  ) {
				LastDrawPos = MouseTilePos;
				if ((selection.Width == 1) && (selection.Height == 1)) Replace(Tilemap[MouseTilePos], selection[0,0]);
			}
			if(selecting)
				UpdateSelection();
			Redraw();
		}
	}

	private bool UpdateMouseTilePos(Vector MousePos)
	{
		FieldPos NewMouseTilePos = new FieldPos(
				(int) (MousePos.X) / 32,
				(int) (MousePos.Y) / 32);
		if(NewMouseTilePos != MouseTilePos) {
			MouseTilePos = NewMouseTilePos;
			return true;
		}

		return false;
	}

	private void UpdateSelection()
	{
		if(MouseTilePos.X < SelectStartPos.X) {
			if(MouseTilePos.X < 0)
				SelectionP1.X = 0;
			else
				SelectionP1.X = MouseTilePos.X;
			SelectionP2.X = SelectStartPos.X;
		} else {
			SelectionP1.X = SelectStartPos.X;
			if(MouseTilePos.X >= Tilemap.Width)
				SelectionP2.X = (int) Tilemap.Width - 1;
			else
				SelectionP2.X = MouseTilePos.X;
		}

		if(MouseTilePos.Y < SelectStartPos.Y) {
			if(MouseTilePos.Y < 0)
				SelectionP1.Y = 0;
			else
				SelectionP1.Y = MouseTilePos.Y;
			SelectionP2.Y = SelectStartPos.Y;
		} else {
			SelectionP1.Y = SelectStartPos.Y;
			if(MouseTilePos.Y >= Tilemap.Height)
				SelectionP2.Y = (int) Tilemap.Height - 1;
			else
				SelectionP2.Y = MouseTilePos.Y;
		}
	}

	private void OnSelectionChanged() {
		Redraw();
	}
}
