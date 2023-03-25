#pragma once
#include <vector>
#include <list>
#include <memory>
#include <stdexcept>
#include <stdexcept>
#include <SDL.h>

namespace FQT {
	struct Point {
		int32_t t, l;
	};

	struct AABB {
		int32_t t, l, b, r;
	};

	struct Element {
		Point point;
		int ext_id;
	};

	struct Node {
		using NodePtr = std::shared_ptr<Node>;

		AABB aabb;
		std::int32_t first, count;
		NodePtr nw, ne, sw, se;
	};

	class ElementList {
	private:
		int groupSize = 0;
		int currentGroupNumber = 0;
		std::vector<Element> elements;
		std::vector<int32_t> clearedGroups;
	public:
		ElementList() {}
		ElementList(int groupSize) {
			this->groupSize = groupSize;
		}

		std::vector<Element> get() {
			return this->elements;
		}

		void insert(Element element, int32_t first, int32_t num) {
			if (first > this->elements.size() || first % this->groupSize != 0 || (num > this->groupSize || num < 0)) {
				throw std::out_of_range("Element group insertion error.");
			} else {
				this->elements.at(first + num) = element;
				return;
			}
		}

		std::vector<Element> clear(int32_t first) {
			if (first > this->elements.size() || first % 4 != 0) {
				throw std::out_of_range("Element group clear error.");
			} else {
				clearedGroups.push_back(first);
				std::vector<Element> ret;
				for (int i = first; i < first + 4; i++) {
					ret.push_back(this->elements.at(i));
				}
				return ret;
			}
		}

		int32_t initElementGroup() {
			if (clearedGroups.size() > 0) {
				int group = this->clearedGroups.back();
				this->clearedGroups.pop_back();
				return group;
			}
			else {
				int group = this->currentGroupNumber;
				this->elements.resize(this->elements.size() + this->groupSize);
				this->currentGroupNumber++;
				return group * this->groupSize;
			}
		}
	};

	class QuadTree {
	private:
		ElementList elements;
		int limitPerNode;
		std::shared_ptr<Node> root;

		bool insertToNode(std::shared_ptr<Node> node, Point point, int ext_id) {
			if (!aabbContainsPoint(node.get()->aabb, point)) {
				return false;
			}
			if (node.get()->first != -1 && node.get()->count < limitPerNode) {
				this->elements.insert(Element{ point, ext_id }, node.get()->first, node.get()->count);
				node.get()->count++;
				return true;
			}

			if (node.get()->nw == NULL) {
				std::vector<Element> deallocatedElements = this->elements.clear(node.get()->first);
				for (Element e : deallocatedElements) {
					if (aabbContainsPoint(nw(node.get()->aabb), e.point)) {
						node.get()->nw.reset(new Node{ nw(node.get()->aabb), this->elements.initElementGroup(), 0 });
						if (insertToNode(node.get()->nw, point, ext_id)) { continue; }
					}
					if (aabbContainsPoint(ne(node.get()->aabb), e.point)) {
						node.get()->ne.reset(new Node{ ne(node.get()->aabb), this->elements.initElementGroup(), 0 });
						if (insertToNode(node.get()->ne, point, ext_id)) { continue; }
					}
					if (aabbContainsPoint(sw(node.get()->aabb), e.point)) {
						node.get()->sw.reset(new Node{ sw(node.get()->aabb), this->elements.initElementGroup(), 0 });
						if (insertToNode(node.get()->sw, point, ext_id)) { continue; }
					}
					if (aabbContainsPoint(se(node.get()->aabb), e.point)) {
						node.get()->se.reset(new Node{ se(node.get()->aabb), this->elements.initElementGroup(), 0 });
						if (insertToNode(node.get()->se, point, ext_id)) { continue; }
					}
				}
				node.get()->first = -1;
				node.get()->count = 0;
			}

			if (aabbContainsPoint(nw(node.get()->aabb), point)) {
				if (node.get()->nw == NULL) {
					node.get()->nw.reset(new Node{ nw(node.get()->aabb), this->elements.initElementGroup(), 0 });
				}
				if (insertToNode(node.get()->nw, point, ext_id)) { return true; }
			}
			if (aabbContainsPoint(ne(node.get()->aabb), point)) {
				if (node.get()->ne == NULL) {
					node.get()->ne.reset(new Node{ ne(node.get()->aabb), this->elements.initElementGroup(), 0 });
				}
				if (insertToNode(node.get()->ne, point, ext_id)) { return true; }
			}
			if (aabbContainsPoint(sw(node.get()->aabb), point)) {
				if (node.get()->sw == NULL) {
					node.get()->sw.reset(new Node{ sw(node.get()->aabb), this->elements.initElementGroup(), 0 });
				}
				if (insertToNode(node.get()->sw, point, ext_id)) { return true; }
			}
			if (aabbContainsPoint(se(node.get()->aabb), point)) {
				if (node.get()->se == NULL) {
					node.get()->se.reset(new Node{ se(node.get()->aabb), this->elements.initElementGroup(), 0 });
				}
				if (insertToNode(node.get()->se, point, ext_id)) { return true; }
			}

			return false;
		}
		AABB nw(AABB aabb) {
			return AABB{ aabb.t, aabb.l, aabb.b / 2, aabb.r / 2 };
		}
		AABB ne(AABB aabb) {
			return AABB{ aabb.t, aabb.l + (aabb.r/2), aabb.b / 2, aabb.r / 2};
		}
		AABB sw(AABB aabb) {
			return AABB{ aabb.t + (aabb.b/2), aabb.l, aabb.b / 2, aabb.r / 2};
		}
		AABB se(AABB aabb) {
			return AABB{ aabb.t + (aabb.b / 2), aabb.l + (aabb.r / 2), aabb.b / 2, aabb.r / 2 };
		}

		void draw(std::shared_ptr<Node> root, SDL_Renderer* renderer) {
			AABB aabb = root.get()->aabb;
			SDL_Rect rect = { aabb.t, aabb.l, aabb.r, aabb.b };
			SDL_SetRenderDrawColor(renderer, 65, 65, 65, 255);
			SDL_RenderDrawRect(renderer, &rect);
			if (root.get()->first == -1) {
				show(root.get()->nw, renderer);
				show(root.get()->ne, renderer);
				show(root.get()->sw, renderer);
				show(root.get()->ne, renderer);
			}
			return;
		}

	public:
		QuadTree(AABB aabb, int limitPerNode) {
			this->limitPerNode = limitPerNode;
			this->elements = ElementList(limitPerNode);
			this->root = std::shared_ptr<Node>(new Node{ aabb, this->elements.initElementGroup(), 0 });
			this->limitPerNode = 4;
		}
		bool insert(int32_t t, int32_t l, int ext_id) {
			Point point = { t, l };
			if (!aabbContainsPoint(this->root.get()->aabb, point)) {
				return false;
			}

			return insertToNode(this->root, point, ext_id);
		}

		std::vector<Element> query(AABB range) {
			// TODO
		}

		static constexpr bool aabbContainsPoint(AABB aabb, Point point) {
			return ((point.l >= aabb.l) && (point.l < aabb.l + aabb.r))
				&& ((point.t >= aabb.t) && (point.t < aabb.t + aabb.b));
		}

		static constexpr bool intersects(AABB aabb1, AABB aabb2) {
			return (aabb1.l <= (aabb2.l + aabb2.r) && (aabb1.l + aabb1.r) > aabb2.l
				&& aabb1.t <= (aabb2.t + aabb2.b) && (aabb1.t + aabb1.b) > aabb2.t);
		}

		void show(std::shared_ptr<Node> root, SDL_Renderer* renderer) {
			if (root == NULL) {
				root = this->root;
			}
			draw(root, renderer);
		}

		void showElements(SDL_Renderer* renderer) {
			for (Element e : this->elements.get()) {
				SDL_Rect rect; 
				rect.x = e.point.l;
				rect.y = e.point.t;
				rect.w = 4;
				rect.h = 4;
				SDL_SetRenderDrawColor(renderer, 234, 234, 234, 255);
				SDL_RenderDrawRect(renderer, &rect);
			}
		}
	};
}