#pragma once
#include <cstdint>
#include "ElementStore.h"
#include <memory>

struct AABB {
	int32_t t, l, b, r;
};

// ---------------------------
// Leaf boundaries of AABBs
// ---------------------------
[[nodiscard]] const static constexpr AABB nw(AABB aabb) noexcept {
	return AABB{ aabb.t, aabb.l, aabb.b / 2, aabb.r / 2 };
}
[[nodiscard]] const static constexpr AABB ne(AABB aabb) noexcept {
	return AABB{ aabb.t, aabb.l + (aabb.r / 2), aabb.b / 2, aabb.r / 2 };
}
[[nodiscard]] const static constexpr AABB sw(AABB aabb) noexcept {
	return AABB{ aabb.t + (aabb.b / 2), aabb.l, aabb.b / 2, aabb.r / 2 };
}
[[nodiscard]] const static constexpr AABB se(AABB aabb) noexcept {
	return AABB{ aabb.t + (aabb.b / 2), aabb.l + (aabb.r / 2), aabb.b / 2, aabb.r / 2 };
}

// ---------------------------
// Overlaps in AABBs
// ---------------------------
const static constexpr bool aabbContainsPoint(AABB aabb, Store::Point point) {
	int32_t x1 = aabb.l; 
	int32_t x2 = aabb.l + aabb.r;
	int32_t y1 = aabb.t;
	int32_t y2 = aabb.t + aabb.b;
	return ((point.x >= x1) && (point.x <= x2))
		&& ((point.y >= y1) && (point.y <= y2));
}
const static constexpr bool intersects(AABB aabb1, AABB aabb2) {
	int32_t ax1 = aabb1.l;
	int32_t ax2 = aabb1.l + aabb1.r;
	int32_t ay1 = aabb1.t;
	int32_t ay2 = aabb1.t + aabb1.b;

	int32_t bx1 = aabb2.l;
	int32_t bx2 = aabb2.l + aabb2.r;
	int32_t by1 = aabb2.t;
	int32_t by2 = aabb2.t + aabb2.b;

	return (ax1 <= bx2 && ax2 >= bx1) && (ay1 <= by2 && ay2 >= by1);
}

struct Node {
	using NodePtr = std::shared_ptr<Node>;
	AABB aabb;
	std::int32_t first, count;
	NodePtr nw, ne, sw, se;
};

class FQT2 {
private:
	Store::ElementStore store;
	const int limit = 4;
	std::shared_ptr<Node> root;

	bool insert(Store::Element element, std::shared_ptr<Node> node) {
		Node* const& nodePtr = node.get();

		if (!aabbContainsPoint(nodePtr->aabb, element.point)) {
			return false;
		}
		if (limit > nodePtr->count && nodePtr->first != -1) {
			this->store.insert(element, nodePtr->first, nodePtr->count);
			nodePtr->count++;
			return true;
		}
		if (node.get()->nw == NULL) {
			subdivide(node);
		}
		if (insert(element, nodePtr->nw)) return true;
		if (insert(element, nodePtr->ne)) return true;
		if (insert(element, nodePtr->sw)) return true;
		if (insert(element, nodePtr->se)) return true;
		return false;
	}
	void subdivide(std::shared_ptr<Node> node) {
		Node* const& nodePtr = node.get();

		// clear elements in parent node
		std::vector<Store::Element> clearedElements = this->store.clear(nodePtr->first);
		nodePtr->first = -1;
		nodePtr->count = 0;

		// initialize child leaves
		nodePtr->nw.reset(new Node{ nw(nodePtr->aabb), this->store.initElementGroup(), 0 });
		nodePtr->ne.reset(new Node{ ne(nodePtr->aabb), this->store.initElementGroup(), 0 });
		nodePtr->sw.reset(new Node{ sw(nodePtr->aabb), this->store.initElementGroup(), 0 });
		nodePtr->se.reset(new Node{ se(nodePtr->aabb), this->store.initElementGroup(), 0 });


		for (Store::Element orphan : clearedElements) {
			if (insert(orphan, nodePtr->nw)) continue;
			if (insert(orphan, nodePtr->ne)) continue;
			if (insert(orphan, nodePtr->sw)) continue;
			if (insert(orphan, nodePtr->se)) continue;
		}
	}
public: 
	FQT2(AABB aabb) {
		this->store = Store::ElementStore(4);
		this->root = std::make_shared<Node>(Node { aabb, this->store.initElementGroup(), 0, 0});
	}
	bool add(Store::Element element) {
		return insert(element, this->root);
	}

	std::vector<int> query(AABB range, std::shared_ptr<Node> node, bool first) {
		if (first) {
			node = this->root;
		}
		std::vector<int> extIds;
		if (!intersects(range, node.get()->aabb)) {
			return extIds;
		}
		if (node.get()->first != -1) {
			for (Store::Element e : this->store.getGroup(node.get()->first)) {
				if (aabbContainsPoint(range, e.point)) {
					extIds.push_back(e.ext_id);
				}
			}
		}
		if (node.get()->first == -1) {
			std::vector<int> nwids = query(range, node.get()->nw, false);
			extIds.insert(extIds.end(), nwids.begin(), nwids.end());

			std::vector<int> neids = query(range, node.get()->ne, false);
			extIds.insert(extIds.end(), neids.begin(), neids.end());

			std::vector<int> swids = query(range, node.get()->sw, false);
			extIds.insert(extIds.end(), swids.begin(), swids.end());

			std::vector<int> seids = query(range, node.get()->se, false);
			extIds.insert(extIds.end(), seids.begin(), seids.end());
		}
		return extIds;
	}

	void draw(SDL_Renderer* renderer) {
		for (Store::Element e : this->store.get()) {
			SDL_Rect r;
			r.x = e.point.x;
			r.y = e.point.y;
			r.w = 2;
			r.h = 2;
			SDL_SetRenderDrawColor(renderer, 165, 165, 165, 255);
			SDL_RenderDrawRect(renderer, &r);
		}
	}
	void show(SDL_Renderer* renderer, std::shared_ptr<Node> node, bool first) {
		if (first) {
			node = this->root;
		}
		AABB aabb = node.get()->aabb;
		SDL_Rect r;
		r.x = aabb.l;
		r.y = aabb.t;
		r.w = aabb.r;
		r.h = aabb.b;
		SDL_SetRenderDrawColor(renderer, 75, 75, 75, 255);
		SDL_RenderDrawRect(renderer, &r);
		if (node.get()->first == -1) {
			show(renderer, node.get()->nw, false);
			show(renderer, node.get()->ne, false);
			show(renderer, node.get()->sw, false);
			show(renderer, node.get()->se, false);
		}
	}
};