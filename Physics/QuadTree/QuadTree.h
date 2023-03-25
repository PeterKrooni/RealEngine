#pragma once
#include <vector>
#include <iostream>
#include <SDL.h>

class UserData {
public:
	int id;
};

struct Point {
	int x;
	int y;
	int body_id;
};

class Rectangle {
public:
	int x;
	int y;
	int w;
	int h;
	bool contains(Point point) {
		return ((point.x >= this->x) && (point.x < this->x + this->w))
			&& ((point.y >= this->y) && (point.y < this->y + this->h));
	}
	bool intersects(Rectangle rectangle) {
		int x1A = this->x;
		int y1A = this->y;
		int x2A = this->x + this->w;
		int y2A = this->y + this->h;
		int x1B = rectangle.x;
		int y1B = rectangle.y;
		int x2B = rectangle.x + rectangle.w;
		int y2B = rectangle.y + rectangle.h;
		return (x1A <= x2B && x2A > x1B 
			 && y1A <= y2B && y2A > y1B);
	}
};

class QuadTree {
private:
	Rectangle boundary;
	int capacity;
	std::vector<Point> points;
	QuadTree* northWest = nullptr;
	QuadTree* northEast = nullptr;
	QuadTree* southWest = nullptr;
	QuadTree* southEast = nullptr;
public:
	QuadTree(Rectangle boundary, int n) {
		if (northWest != nullptr) {
			int i = 0;
		}
		this->boundary = boundary;
		this->capacity = n;
	}
	~QuadTree() {
		//delete northWest;
		this->northWest = NULL;
		this->northEast = NULL;
		this->southWest = NULL;
		this->southEast = NULL;
	}

	bool insert(Point point) {
		if (!this->boundary.contains(point)) {
			return false;
		}
		if (this->points.size() < this->capacity && this->northWest == nullptr) {
			this->points.push_back(point);
			return true;
		} 
		if (this->northWest == nullptr) {
			this->subdivide();
		}
		for (Point p : this->points) {
			if (this->northWest->insert(p)) { continue; }
			if (this->northEast->insert(p)) { continue; }
			if (this->southWest->insert(p)) { continue; }
			if (this->southEast->insert(p)) { continue; }
		}
		points.clear();
		if (this->northWest->insert(point)) { return true; }
		if (this->northEast->insert(point)) { return true; }
		if (this->southWest->insert(point)) { return true; }
		if (this->southEast->insert(point)) { return true; }

		return false;
	}
	void subdivide() {
		int x = this->boundary.x;
		int y = this->boundary.y;
		int w = this->boundary.w;
		int h = this->boundary.h;
		Rectangle nw = Rectangle{ x, y, (w / 2), (h / 2) };
		Rectangle ne = Rectangle{ x + (w / 2), y, (w / 2), (h / 2) };
		Rectangle sw = Rectangle{ x, y + (h / 2), (w / 2), (h / 2) };
		Rectangle se = Rectangle{ x + (w / 2), y + (h / 2), (w / 2), (h / 2) };

		this->northWest = new QuadTree(nw, this->capacity);
		this->northEast = new QuadTree(ne, this->capacity);
		this->southWest = new QuadTree(sw, this->capacity);
		this->southEast = new QuadTree(se, this->capacity);
	}
	
	std::vector<Point> query(Rectangle boundary) {
		std::vector<Point> found;
		if (!this->boundary.intersects(boundary)) {
			return found;
		}
		for (Point p : this->points) {
			if (boundary.contains(p)) {
				found.push_back(p);
			}
		}
		if (this->northWest == nullptr) {
			return found;
		}
		std::vector<Point> nwPoints = this->northWest->query(boundary);
		found.insert(found.end(), nwPoints.begin(), nwPoints.end());

		std::vector<Point> nePoints = this->northEast->query(boundary);
		found.insert(found.end(), nePoints.begin(), nePoints.end());

		std::vector<Point> swPoints = this->southWest->query(boundary);
		found.insert(found.end(), swPoints.begin(), swPoints.end());

		std::vector<Point> sePoints = this->southEast->query(boundary);
		found.insert(found.end(), sePoints.begin(), sePoints.end());
		return found;
	}

	std::vector<Point> queryNode(int id) {

	}

	int getPointCount() {
		int points = 0;
		points += this->points.size();
		if (this->northWest != nullptr) {
			points += this->northWest->getPointCount();
		}
		if (this->northEast != nullptr) {
			points += this->northEast->getPointCount();
		}
		if (this->southWest != nullptr) {
			points += this->southWest->getPointCount();
		}
		if (this->southEast != nullptr) {
			points += this->southEast->getPointCount();
		}
		return points;
	}

	int show(SDL_Renderer* renderer) {
		int max_depth = 1;
		

		SDL_Rect rect = getRect();
		SDL_SetRenderDrawColor(renderer, 65, 65, 65, 255);
		SDL_RenderDrawRect(renderer, &rect);
		if (this->northWest != nullptr) {
			max_depth += this->northWest->show(renderer);
			this->northEast->show(renderer);
			this->southWest->show(renderer);
			this->southEast->show(renderer);
		}
		return max_depth;
	}
	SDL_Rect getRect() {
		return SDL_Rect{ this->boundary.x, this->boundary.y, this->boundary.w, this->boundary.h };
	}
};