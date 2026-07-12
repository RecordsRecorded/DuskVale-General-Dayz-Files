class MapMarkerWrapperCircle : MapMarkerWrapper {

	const int CIRCLE_WIDTH = 2;

	float radius = 100.0;
	ref array<MapMarkerWrapperCircle> otherCircles = new array<MapMarkerWrapperCircle>();
	bool needPointUpdate = true;
	bool drawLines = true, fullyInsideOtherCircle = false;
	ref array<ref Param2<float, float>> intersectingAngles = new array<ref Param2<float, float >> ();
	float lastcircumference = -1;
	ref array<ref Param4<bool, bool, float, bool>> intersectionPreCalculated = new array<ref Param4<bool, bool, float, bool >> ();

	void Init(vector center, float radius_, int color, bool striked) {
		this.drawLines = striked;
		this.position = center;
		this.radius = radius_;
		this.lastColor = color;
		drawCanvas.GetScreenSize(widgetWidth, widgetHeight);
	}

	bool SetStriked(bool striked) {
		if (drawLines != striked) {
			drawLines = striked;
			changed = true;
			return true;
		}
		return false;
	}

	override bool IsVisible() {
		return drawCanvas && drawCanvas.IsVisible();
	}

	override void Update(float mapScale, vector mapPos, MapWidget mapWidget, float xOffset, float yOffset) {
		float canvasWidth, canvasHeight;
		drawCanvas.GetScreenSize(canvasWidth, canvasHeight);

		vector screenPos = mapWidget.MapToScreen(position) + Vector(-xOffset, -yOffset, 0);

		float radiusRoot = Math.Sqrt(radius);
		float circumference = 2.0 * Math.PI * radiusRoot;
		if (radiusRoot <= 0)
			return;

		float part = 1.0 / radiusRoot;
		float height = LBWidgetUtils.screenHeight * 1.33333333;
		if (height > LBWidgetUtils.screenWidth)
			height = LBWidgetUtils.screenWidth;
		float screenScale = height / g_Game.GetWorld().GetWorldSize();
		float mapToScreen = mapScale / screenScale;
		float radiusScreen = radius / mapToScreen;

		if (screenPos[0] + radiusScreen < 0 || screenPos[0] - radiusScreen > canvasWidth || screenPos[1] + radiusScreen < 0 || screenPos[1] - radiusScreen > canvasHeight) {
			return;
		}
		if (fullyInsideOtherCircle)
			return;

		float angle2 = 0.0;
		float rawX2 = radius, rawY2 = 0.0, newY2 = screenPos[1], newX2 = screenPos[0] + radius / mapToScreen;
		int partStart = TickCount(0);
		if (needPointUpdate) {
			CalcAllCircleIntersections();
		}
		if (needPointUpdate || lastcircumference != circumference) {
			CalcCircleIntersections(circumference, part);
			lastcircumference = circumference;
		}
		needPointUpdate = false;
		bool hasIntersections = false;
		if (intersectingAngles.Count() > 0)
			hasIntersections = true;
		int currentIntersectionIndex = 0;

		DrawStrikeLines(mapToScreen, mapWidget, xOffset, yOffset, lastColor);
		int i = 0;
		foreach (Param4<bool, bool, float, bool> param : intersectionPreCalculated) {
			i++;
			float angle1 = angle2;
			angle2 = part * i;
			float rawX1 = rawX2;
			rawX2 = radius * Math.Cos(angle2);
			float rawY1 = rawY2;
			rawY2 = -radius * Math.Sin(angle2);
			float newX1 = newX2;
			newX2 = screenPos[0] + rawX2 / mapToScreen;
			float newY1 = newY2;
			newY2 = screenPos[1] + rawY2 / mapToScreen;

			if (!hasIntersections || currentIntersectionIndex >= intersectingAngles.Count()) {
				drawCanvas.DrawLine(newX1, newY1, newX2, newY2, CIRCLE_WIDTH, lastColor);
				hasIntersections = false;
				continue;
			}

			if (param.param1) {
				continue;
			}
			if (param.param2) {
				if (param.param4) {
					float newX3 = screenPos[0] + radius * Math.Cos(param.param3 + 0.0015) / mapToScreen;
					float newY3 = screenPos[1] - radius * Math.Sin(param.param3 + 0.0015) / mapToScreen;
					drawCanvas.DrawLine(newX3, newY3, newX2, newY2, CIRCLE_WIDTH, lastColor);
				} else {
					newX3 = screenPos[0] + radius * Math.Cos(param.param3 - 0.0015) / mapToScreen;
					newY3 = screenPos[1] - radius * Math.Sin(param.param3 - 0.0015) / mapToScreen;
					drawCanvas.DrawLine(newX3, newY3, newX1, newY1, CIRCLE_WIDTH, lastColor);
				}
				continue;
			}
			drawCanvas.DrawLine(newX1, newY1, newX2, newY2, CIRCLE_WIDTH, lastColor);
		}
	}

	void CalcCircleIntersections(int circumference, float part) {
		intersectionPreCalculated.Clear();
		float angle2 = 0.0;
		for (int i = 1; i < circumference + 2; i++) {

			if (intersectingAngles.Count() <= 0) {
				intersectionPreCalculated.Insert(new Param4<bool, bool, float, bool>(false, false, 0.0, false));
				continue;
			}
			float angle1 = angle2;
			angle2 = part * i;

			Param2<float, float> inter = intersectingAngles.Get(0);
			bool intersecting = false;
			bool edge = false;
			float angleEdge = 0.0;
			bool useNew2 = false;
			foreach (Param2<float, float> intersects : intersectingAngles) {
				if (angle2 > intersects.param1 && angle1 < intersects.param2) {
					if (intersects.param1 < angle1) {
						angleEdge = intersects.param2;
						useNew2 = true;
					} else {
						angleEdge = intersects.param1;
						useNew2 = false;
					}
					edge = true;
				}
				if (angle1 > intersects.param1 && angle2 < intersects.param2) {
					intersecting = true;
					break;
				}
			}
			intersectionPreCalculated.Insert(new Param4<bool, bool, float, bool>(intersecting, edge, angleEdge, useNew2));
		}
	}

	void DrawStrikeLines(float mapToScreen, MapWidget mapWidget, float xOffset, float yOffset, int color) {
		int startTime = TickCount(0);
		if (!drawLines)
			return;

		vector screenPos = mapWidget.MapToScreen(position) + Vector(-xOffset, -yOffset, 0);
		float screenPos1 = screenPos[0];
		float screenPos2 = screenPos[1];
		float radiusScreen = radius / mapToScreen;

		int lineCount = (radiusScreen * 2) / 20 + 1;
		int radPart = radiusScreen / 20;
		float sq_20_2 = 20.0 / Math.Sqrt(2.0);
		float screen_X = screenPos[0] / 2.0;
		float start_Offset = (screen_X - ((int)(screen_X / sq_20_2)) * sq_20_2);
		float screen_Y = screenPos[1] / 2.0;
		start_Offset = start_Offset + (screen_Y - ((int)(screen_Y / sq_20_2)) * sq_20_2);
		float start1 = radPart * sq_20_2 - start_Offset + sq_20_2;
		float start2 = radPart * sq_20_2 - start_Offset + sq_20_2;
		if (lineCount > 1000) {
			return;
		}
		int strikeColorRGB = color & 0x00ffffff;
		int strikeColorA = ((color & 0xff000000) >> 1) & 0x7f000000;

		for (int i = 1; i <= lineCount; i++) {
			float otherLen = Math.Sqrt(radiusScreen * radiusScreen - (start1 * start1 + start2 * start2));
			float begin1 = 0.7071067 * otherLen;
			float begin2 = -0.7071067 * otherLen;
			float end1 = screenPos1 + start1 - begin1;
			float end2 = screenPos2 + start2 - begin2;
			begin1 = begin1 + screenPos1 + start1;
			begin2 = begin2 + screenPos2 + start2;
			start1 -= sq_20_2;
			start2 -= sq_20_2;

			drawCanvas.DrawLine(begin1, begin2, end1, end2, CIRCLE_WIDTH, strikeColorA | strikeColorRGB);
		}
	}

	bool SetRadius(float radius_) {
		if (radius_ != radius) {
			this.radius = radius_;
			changed = true;
			needPointUpdate = true;
			CalculateInsideOtherCircle();
			return true;
		}
		return false;
	}

	void CalcAllCircleIntersections() {
		intersectingAngles.Clear();
		if (!otherCircles)
			return;

		for (int i = 0; i < otherCircles.Count(); i++) {
			MapMarkerWrapperCircle circle = otherCircles.Get(i);
			if (circle == this) {
				otherCircles.Remove(i);
				i--;
				continue;
			}
			float d = Math.Sqrt((position[0] - circle.position[0]) * (position[0] - circle.position[0]) + (position[2] - circle.position[2]) * (position[2] - circle.position[2]));
			if (d >= radius + circle.radius || d <= 0) {
				otherCircles.Remove(i);
				i--;
				continue;
			}
			float a = (radius * radius - circle.radius * circle.radius + d * d) / 2 / d;
			float h = Math.Sqrt(radius * radius - a * a);
			float hd = h / d;
			float x3 = position[0] + (circle.position[0] - position[0]) * a / d;
			float y3 = position[2] + (circle.position[2] - position[2]) * a / d;

			float x1 = (x3 + hd * (circle.position[2] - position[2]) - position[0]);
			float y1 = (y3 - hd * (circle.position[0] - position[0]) - position[2]);

			float x2 = (x3 - hd * (circle.position[2] - position[2]) - position[0]);
			float y2 = (y3 + hd * (circle.position[0] - position[0]) - position[2]);

			float dist1 = Math.Sqrt(y1 * y1 + x1 * x1);
			float dist2 = Math.Sqrt(y2 * y2 + x2 * x2);
			if (dist1 <= 0 || dist2 <= 0) {
				otherCircles.Remove(i);
				i--;
				continue;
			}
			float angle1 = Math.Acos(x1 / dist1);
			if (y1 < 0)
				angle1 = Math.PI2 - angle1;
			float angle2 = Math.Acos(x2 / dist2);
			if (y2 < 0)
				angle2 = Math.PI2 - angle2;
#ifdef DIAG
			LBLogger.Verbose("Angles for " + position + ": " + angle1 + " " + angle2 + " " + y1 + " " + x1 + "   " + y2 + " " + x2, "AdvancedGroups");
#endif
			if (angle1 < angle2) {
				intersectingAngles.Insert(new Param2<float, float>(angle1, angle2));
			} else {
				intersectingAngles.Insert(new Param2<float, float>(-1, angle2));
				intersectingAngles.Insert(new Param2<float, float>(angle1, Math.PI2 + 1));
			}
		}
	}

	void CalculateInsideOtherCircle() {
		fullyInsideOtherCircle = false;
		foreach (MapMarkerWrapperCircle circ : otherCircles) {
			if (circ == this)
				continue;
			float x = circ.position[0] - position[0];
			float z = circ.position[2] - position[2];
			float distance = Math.Sqrt(x * x + z * z);
			float min = distance + radius;
			if (min < circ.radius) {
				fullyInsideOtherCircle = true;
				return;
			}
		}
	}

	void SetOtherCircles(array<MapMarkerWrapperCircle> circles) {
		otherCircles.Clear();
		foreach (MapMarkerWrapperCircle circ : circles) {
			otherCircles.Insert(circ);
		}
		needPointUpdate = true;
		CalculateInsideOtherCircle();
	}
}
