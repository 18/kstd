#pragma once
#include <memory>
#include <utility>
#include <type_traits>
#include <stdexcept>
#include "kmemory.h"

namespace kstd
{
  namespace detail
  {
    template<typename T, typename = void>
    struct allocator_base
    {
    public:
      T get_allocator() const noexcept
      {
        return allocator_;
      }
    protected:
      T& allocator() noexcept
      {
        return allocator_;
      }

      const T& allocator() const noexcept
      {
        return allocator_;
      }
    private:
      T allocator_;
    };

    template<typename T>
    struct allocator_base<T, std::enable_if_t<!std::is_final_v<T>>> : T
    {
    public:
      T get_allocator() const noexcept
      {
        return allocator();
      }
    protected:
      T& allocator() noexcept
      {
        return *static_cast<T*>(this);
      }

      const T& allocator() const noexcept
      {
        return *static_cast<T*>(this);
      }
    };
  }

  template<typename T, typename Allocator = std::allocator<T>>
  class vector : public detail::allocator_base<Allocator>
  {
  public:
    // typedefs
    using value_type = T;
    using allocator_type = Allocator;
    using pointer = typename std::allocator_traits<Allocator>::pointer;
    using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;
    using reference = value_type&;
    using const_reference = const value_type&;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using iterator = pointer;
    using const_iterator = const_pointer;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    // constructors

    // iterators
    iterator begin() noexcept
    {
      return data_;
    }

    const_iterator begin() const noexcept
    {
      return data_;
    }

    iterator end() noexcept
    {
      return data_ + size_;
    }

    const_iterator end() const noexcept
    {
      return data_ + size_;
    }

    reverse_iterator rbegin() noexcept
    {
      return data_ + size_;
    }

    const_reverse_iterator rbegin() const noexcept
    {
      return data_ + size_;
    }

    reverse_iterator rend() noexcept
    {
      return data_;
    }

    const_reverse_iterator rend() const noexcept
    {
      return data_;
    }

    const_iterator cbegin() const noexcept
    {
      return data_;
    }

    const_iterator cend() const noexcept
    {
      return data_ + size_;
    }

    const_reverse_iterator crbegin() const noexcept
    {
      return data_ + size_;
    }

    const_reverse_iterator crend() const noexcept
    {
      return data_;
    }

    // capacity
    bool empty() const noexcept
    {
      return !size_;
    }

    size_type size() const noexcept
    {
      return size_;
    }

    size_type capactiy() const noexcept
    {
      return capacity_;
    }

    void resize(size_type sz)
    {

    }

    void resize(size_type sz, const T& value)
    {

    }

    void reserve(size_type cap)
    {
      reserve_offset(cap, size_, 0);
    }

    void shrink_to_fit()
    {
      return;
    }

    // element access
    reference operator[](size_type n)
    {
      return data_[n];
    }
    
    const_reference operator[](size_type n) const
    {
      return data_[n];
    }
    
    reference at(size_type n)
    {
      if (n >= size_)
        throw std::out_of_range("n is out of range");
      return data_[n];
    }
    
    const_reference at(size_type n) const
    {
      return at(n);
    }
    
    reference front()
    {
      return *data_;
    }
    
    const_reference front() const
    {
      return *data_;
    }
    
    reference back()
    {
      return data_[size_ - 1];
    }
    
    const_reference back() const
    {
      return data_[size_ - 1];
    }

    // data access
    T* data() noexcept
    {
      return data_;
    }

    const T* data() const noexcept
    {
      return data_;
    }

    // modifiers
    template<typename... Args>
    reference emplace_back(Args&&... args)
    {
      return *emplace(end(), std::forward<Args>(args)...);
    }

    void push_back(const T& value)
    {
      emplace_back(value);
    }

    void push_back(T&& value)
    {
      emplace_back(std::move(value));
    }

    void pop_back()
    {
      if (!std::is_trivially_destructible_v<T>)
        back().~T();
      --size_;
    }

    template<typename... Args> 
    iterator emplace(const_iterator pos, Args&&... args)
    {
      size_type emplaced_pos = pos - begin();
      if (!shift_elements_right(emplaced_pos, 1) && emplaced_pos < size_)
        *(data_ + emplaced_pos) = T(std::forward<Args>(args)...);
      else
        new (data_ + emplaced_pos) T(std::forward<Args>(args)...);
      ++size_;
      return data_ + emplaced_pos;
    }

    iterator insert(const_iterator pos, const T& value)
    {
      return emplace(pos, value);
    }

    iterator insert(const_iterator pos, T&& value)
    {
      return emplace(pos, std::move(value));
    }

    iterator insert(const_iterator pos, size_type count, const T& value)
    {
      size_type emplaced_pos = pos - begin();
      if (!shift_elements_right(emplaced_pos, count))
      {
        size_type uninit_to_copy = std::clamp((emplaced_pos + count) - size_, size_type(0), count);
        detail::fill_range_optimal(data_ + emplaced_pos, data_ + emplaced_pos + count - uninit_to_copy, value);
        detail::uninitialized_fill_range_optimal(data_ + emplaced_pos + count - uninit_to_copy, data_ + emplaced_pos + count, value);
      }
      else
      {
        detail::uninitialized_fill_range_optimal(data_ + emplaced_pos, data_ + emplaced_pos + count, value);
      }
      size_ += count;
      return data_ + emplaced_pos;
    }

    template<typename InputIt>
    iterator insert(const_iterator pos, InputIt first, InputIt last)
    {
      size_type emplaced_pos = pos - begin();
      size_type count = last - first;
      if (!shift_elements_right(emplaced_pos, count))
      {
        size_type uninit_to_copy = std::clamp((emplaced_pos + count) - size_, size_type(0), count);
        detail::copy_range_optimal(first, last - uninit_to_copy, data_ + emplaced_pos);
        detail::uninitialized_copy_range_optimal(last - uninit_to_copy, last, data_ + emplaced_pos + count - uninit_to_copy);
      }
      else
      {
        detail::uninitialized_copy_range_optimal(first, last, data_ + emplaced_pos);
      }
      size_ += count;
      return data_ + emplaced_pos;
    }

    iterator insert(const_iterator pos, std::initializer_list<T> list)
    {
      return insert(pos, list.begin(), list.end());
    }

    iterator erase(const_iterator first, const_iterator last)
    {
      size_type count = last - first;
      size_type pos = first - begin();
      detail::move_range_optimal(data_ + pos + count, data_ + size_, data_ + pos);
      std::destroy(data_ + size_ - count, data_ + size_);
      size_ -= count;
      return data_ + pos + 1;
    }

    iterator erase(const_iterator pos)
    {
      return erase(pos, pos + 1);
    }

    void clear() noexcept
    {
      erase(begin(), end());
    }
  private:
    bool shift_elements_right(size_type pos, size_type count)
    {
      bool realloc = reserve_offset(size_ + count, pos, count);
      if (!realloc)
      {
        size_type uninit_to_move = std::clamp(size_ - pos, size_type(0), count);
        detail::uninitialized_move_range_optimal(data_ + size_ - uninit_to_move, data_ + size_, data_ + size_ + count - uninit_to_move);
        detail::move_range_optimal_backward(data_ + pos, data_ + size_ - uninit_to_move, data_ + pos + count);
      }
      return realloc;
    }

    bool reserve_offset(size_type cap, size_type pos, size_type count)
    {
      if (cap <= capacity_)
        return false;
      if (data_ != nullptr)
      {
        pointer new_data = std::allocator_traits<Allocator>::allocate(allocator(), cap);
        detail::uninitialized_move_range_optimal(data_, data_ + pos, new_data);
        detail::uninitialized_move_range_optimal(data_ + pos, data_ + size_, new_data + pos + count);
        if constexpr (!std::is_trivial_v<T>)
          std::destroy(data_, data_ + size_);
        std::allocator_traits<Allocator>::deallocate(allocator(), data_, capacity_);
        data_ = new_data;
      }
      else
      {
        data_ = std::allocator_traits<Allocator>::allocate(allocator(), cap);
      }
      capacity_ = cap;
      return true;
    }
    using detail::allocator_base<Allocator>::allocator;

    pointer data_ = nullptr;
    size_type size_ = 0;
    size_type capacity_ = 0;
  };
}