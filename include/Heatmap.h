#pragma once
#include "heatmap.c"
#include "colorschemes.h"
#include <iostream>
#include <vector>
#include <cassert>

using namespace std;

namespace heatmap {

	struct Size {
		int w;
		int h;
		Size(int _w, int _h) :w(_w), h(_h) {}
	};

	struct Area
	{
		int minX;
		int minY;
		int maxX;
		int maxY;
		Area(int min_x, int min_y, int max_x, int max_y) :
			minX(min_x), minY(min_y), maxX(max_x), maxY(max_y) {}
		bool operator == (const Area tmp) const
		{
			return this->maxX == tmp.maxX && this->maxY == tmp.maxY &&
				this->minX == tmp.minX && this->minY == tmp.minY;
		}
	};

	template <typename T>
	float* GetPoints(vector<T>& bboxes) {
		int cpoints = bboxes.size() * 2;
		float* points = new float[cpoints];
		int idx = 0;
		for (T box : bboxes) {
			points[idx++] = box.x+box.w/2;
			points[idx++] = box.y+box.h/2;
		}
		return points;
	}

	template <typename T>
	int GetPointsLength(vector<T>& bboxes) {
		return bboxes.size() * 2;
	}

	int* GetScheme(string scheme_str) {
		assert(colorschemes::valied_scheme(scheme_str));
		auto select_scheme = colorschemes::schemes[scheme_str];
		int length = select_scheme.size() * 3;
		int* scheme = new int[length];

		int idx = 0;
		//for (auto rgb = select_scheme.rbegin(); rgb != select_scheme.rend(); ++rgb) {
		for (auto rgb : select_scheme){
			scheme[idx++] = rgb[2];//(*rgb)[0];
			scheme[idx++] = rgb[1];//(*rgb)[1];
			scheme[idx++] = rgb[0];//(*rgb)[2];
		}

		/*
		for (auto rgb = select_scheme.rbegin(); rgb != select_scheme.rbegin();rgb--) {
		for (auto rgb : select_scheme){
			scheme[idx++] = rgb[0];
			scheme[idx++] = rgb[1];
			scheme[idx++] = rgb[2];
		}*/
		return scheme;
	}

	template<typename T>
	unsigned char* heatmap(vector<T> points, int dotsize = 150, int opacity = 128,
		Size size = Size(1024, 1024), string scheme = "classic", Area area = Area(0, 0, 0, 0)) {
		for (auto& box : points) box.y = size.h - box.y - box.h;
		Area tmp_area = Area(0, 0, 0, 0);
		int tmp_override;
		if (area == Area(0, 0, 0, 0)) {
			//tmp_area = Area(0, 0, 0, 0);
			tmp_override = 0;
		}
		else {
			tmp_area = area;
			tmp_override = 1;
		}

		auto tmp_scheme = move(GetScheme(scheme));
		auto tmp_points = move(GetPoints(points));
		auto tmp_cpoints = GetPointsLength(points);

		unsigned char* pix_color = new unsigned char[size.w * size.h * 4];
		tx(tmp_points, tmp_cpoints, size.w, size.h, dotsize, tmp_scheme, pix_color, opacity, tmp_override,
			tmp_area.minX, tmp_area.minY, tmp_area.maxX, tmp_area.maxY);

		delete tmp_scheme;
		delete tmp_points;

		return pix_color;
	}
}

