## 选项 `--use-deque`

**功能**

使用 `std::deque` 作为 `list` 的目标类型。

**inspiration**

参考 [`std::deque`](https://zh.cppreference.com/w/cpp/container/deque) 上的描述：

> deque 上常见操作的复杂度（效率）如下：
> - 随机访问——常数 O(1)
> - 在结尾或起始插入或移除元素——常数 O(1)
> - 插入或移除元素——线性 O(n)

而 `std::list` 的实现通常为双向链表，支持常数时间从容器任何位置插入和移除元素。

对于 Isabelle/HOL 中的 `list` 而言，常见的操作为从起始移除元素，这点两者的效率是近似的，而拷贝时，`std::deque` 的实现通常为固定数组长度数组的序列，要优于完全不连续的 `std::list`。同时因为 `std::deque` 支持常数时间的随机访问，因此对于 `nth` 等运算，使用 `std::deque` 时的效率也应该是更高的。
