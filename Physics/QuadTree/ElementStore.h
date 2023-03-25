#pragma once
#include <vector>
#include <stdexcept>

namespace Store {
	struct Point {
		int32_t x, y;
	};

	struct Element {
		Point point;
		int ext_id;
	};

	class ElementStore {
	private:
		size_t groupSize = 0;
		int currentGroupNumber = 0;
		std::vector<Element> elements;
		std::vector<int32_t> clearedGroups;
	public:
		ElementStore() {}
		ElementStore(size_t groupSize) {
			this->groupSize = groupSize;
		}

		std::vector<Element> get() {
			return this->elements;
		}

		std::vector<Element> getGroup(int32_t group) {
			std::vector<Element> groupVec;
			for (int i = group; i < group + this->groupSize; i++) {
				Element el = this->elements.at(i);
				if (el.ext_id != -1) {
					groupVec.push_back(el);
				}
			}
			return groupVec;
		}

		void insert(Element element, int32_t first, int32_t num) {
			if (first <= this->elements.size() && first % this->groupSize == 0 && (num < this->groupSize && num >= 0)) {
				this->elements.at(first + num) = element;
				return;
			}
			else {
				throw std::out_of_range("Element group insertion error.");
			}
		}

		std::vector<Element> clear(int32_t first) {
			if (first <= this->elements.size() && first % 4 == 0) {
				clearedGroups.push_back(first);
				std::vector<Element> ret;
				for (int i = first; i < first + 4; i++) {
					if (this->elements.at(i).ext_id != -1) {
						ret.push_back(this->elements.at(i));
					}
					this->elements.at(i).ext_id = -1;
				}
				return ret;
			}
			else {
				throw std::out_of_range("Element group clear error.");
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
}