class LBAdminDrawCanvas {

	static CanvasWidget canvas;
	static ref array<ref LBAdminDrawCanvasObject> objects = new array<ref LBAdminDrawCanvasObject>();

	static void Init() {
		if (canvas)
			canvas.Unlink();
		canvas = CanvasWidget.Cast(LBLayoutManager.Get().CreateLayout("DrawCanvas"));
	}

	static void Draw(LBAdminDrawCanvasObject object, LBAdminDrawCanvasLayer layer) {
		object.SetLayer(layer);
		objects.Insert(object);
	}

	static void Update() {
		canvas.Clear();
		foreach (LBAdminDrawCanvasObject obj : objects)
			obj.Draw(canvas);
	}

	static void Clear() {
		objects.Clear();
	}

	static void ClearLayer(LBAdminDrawCanvasLayer layer) {
		for (int i = 0; i < objects.Count(); ++i) {
			if (objects.Get(i).layer == layer) {
				objects.Remove(i);
				--i;
			}
		}
	}

}