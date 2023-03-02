#include <list>
#include <deque>

// merge sort algorithm for std::list<std::uint64_t>
std::list<std::uint64_t> merge_r(std::list<std::uint64_t> &left, std::list<std::uint64_t> &right) {
    std::list<std::uint64_t> result;
    while (!left.empty() && !right.empty()) {
        if (left.front() <= right.front()) {
            result.push_back(left.front());
            left.pop_front();
        } else {
            result.push_back(right.front());
            right.pop_front();
        }
    }
    while (!left.empty()) {
        result.push_back(left.front());
        left.pop_front();
    }
    while (!right.empty()) {
        result.push_back(right.front());
        right.pop_front();
    }
    return result;
}

std::list<std::uint64_t> merge_sort_r(std::list<std::uint64_t> &input) {
    if (input.size() <= 1) {
        return input;
    }
    std::list<std::uint64_t> left, right, result;
    auto middle = input.begin();
    std::advance(middle, input.size() / 2);
    for (auto it = input.begin(); it != middle; it++) {
        left.push_back(*it);
    }
    for (auto it = middle; it != input.end(); it++) {
        right.push_back(*it);
    }
    left = merge_sort_r(left);
    right = merge_sort_r(right);
    result = merge_r(left, right);
    return result;
}

// merge sort algorithm for std::deque<std::uint64_t>
std::deque<std::uint64_t> merge_r_ud(std::deque<std::uint64_t> &left, std::deque<std::uint64_t> &right) {
    std::deque<std::uint64_t> result;
    while (!left.empty() && !right.empty()) {
        if (left.front() <= right.front()) {
            result.push_back(left.front());
            left.pop_front();
        } else {
            result.push_back(right.front());
            right.pop_front();
        }
    }
    while (!left.empty()) {
        result.push_back(left.front());
        left.pop_front();
    }
    while (!right.empty()) {
        result.push_back(right.front());
        right.pop_front();
    }
    return result;
}

std::deque<std::uint64_t> merge_sort_r_ud(std::deque<std::uint64_t> &input) {
    if (input.size() <= 1) {
        return input;
    }
    std::deque<std::uint64_t> left, right, result;
    auto middle = input.begin();
    std::advance(middle, input.size() / 2);
    for (auto it = input.begin(); it != middle; it++) {
        left.push_back(*it);
    }
    for (auto it = middle; it != input.end(); it++) {
        right.push_back(*it);
    }
    left = merge_sort_r_ud(left);
    right = merge_sort_r_ud(right);
    result = merge_r_ud(left, right);
    return result;
}
