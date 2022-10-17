#include <limits>
#include <vector>
#include <cmath>
#include <algorithm>

template<typename T>
class NumListStats {
    static_assert(std::is_floating_point<T>::value);
private:
    std::vector<T> list;
    std::size_t index;
    std::size_t count;

    T min;
    T max;
public:
    // Default, empty constructor
    NumListStats() {}

    explicit NumListStats(std::size_t count) : list(std::vector<T>(count))
    {
        this->reset();
    }

    void addNum(T num)
    {
        assert(this->list.size() > 0);

        list[index] = num;
        index += 1;
        if (index >= list.size()) {
            index = 0;
        }

        if (count < list.size()) {
            count += 1;
        }

        this->min = std::min(this->min, num);
        this->max = std::max(this->max, num);
    }

    T getMin()
    {
        return this->min;
    }

    T getMax()
    {
        return this->max;
    }

    std::size_t getCount()
    {
        return this->count;
    }

    std::size_t getSize()
    {
        return this->list.size();
    }

    T getAvg()
    {
        assert(this->list.size() > 0);
        T ret = 0;
        for (std::size_t i = 0; i < this->count; ++i) {
            ret += this->list[i];
        }
        return ret/static_cast<T>(this->count);
    }

    T getStandardDeviation()
    {
        assert(this->list.size() > 0);
        T mean = this->getAvg();
        T squaredDiffSum = 0;
        for (std::size_t i = 0; i < this->count; ++i) {
            T val = this->list[i] - mean;
            squaredDiffSum += val * val;
        }
        return std::sqrt(squaredDiffSum / static_cast<T>(this->count));
    }

    void reset()
    {
        this->min = std::numeric_limits<T>::max();
        this->max = std::numeric_limits<T>::min();
        this->index = 0;
        this->count = 0;
    }
};
