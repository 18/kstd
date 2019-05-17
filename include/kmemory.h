#pragma once
#include <memory>
#include <utility>
#include <type_traits>

namespace kstd
{
  namespace detail
  {
    //template<typename T, typename U>
    //T* uninitialized_move_range_optimal(T* first, T* last, U* d_first) // pointers so it works with memcpy
    //{
    //  if constexpr (std::is_trivial_v<T>)
    //    return static_cast<T*>(std::memcpy(d_first, first, (last - first) * sizeof(T)));
    //  else if constexpr (std::is_nothrow_move_constructible_v<T>)
    //    return std::uninitialized_move(first, last, d_first);
    //  else
    //    return std::uninitialized_copy(first, last, d_first);
    //}

    //template<typename T, typename U>
    //T* move_range_optimal(T* first, T* last, U* d_first) // pointers so it works with memcpy
    //{
    //  if constexpr (std::is_trivial_v<T>)
    //    return static_cast<T*>(std::memcpy(d_first, first, (last - first) * sizeof(T)));
    //  else if constexpr (std::is_nothrow_move_constructible_v<T>)
    //    return std::move(first, last, d_first);
    //  else
    //    return std::copy(first, last, d_first);
    //}

    //template<typename T, typename U>
    //T* move_range_optimal_backward(T* first, T* last, U* d_first) // pointers so it works with memcpy
    //{
    //  if constexpr (std::is_trivial_v<T>)
    //    return static_cast<T*>(std::memmove(d_first, first, (last - first) * sizeof(T)));
    //  else if constexpr (std::is_nothrow_move_constructible_v<T>)
    //    return std::move_backward(first, last, d_first);
    //  else
    //    return std::copy_backward(first, last, d_first);
    //}

    //template<typename T, typename U>
    //T* copy_range_optimal(T* first, T* last, U* d_first) // pointers so it works with memcpy
    //{
    //  if constexpr (std::is_trivial_v<T>)
    //    return static_cast<T*>(std::memcpy(d_first, first, (last - first) * sizeof(T)));
    //  else
    //    return std::copy(first, last, d_first);
    //}

    //template<typename T, typename U>
    //T* uninitialized_copy_range_optimal(T* first, T* last, U* d_first) // pointers so it works with memcpy
    //{
    //  if constexpr (std::is_trivial_v<T>)
    //    return static_cast<T*>(std::memcpy(d_first, first, (last - first) * sizeof(T)));
    //  else
    //    return std::uninitialized_copy(first, last, d_first);
    //}

    /////////////////////////////////////////////////////////////////////////////////////////




    template<typename InputIterator, typename OutputIterator>
    OutputIterator uninitialized_move_range_optimal(InputIterator first, InputIterator last, OutputIterator d_first)
    {
      using T = typename std::iterator_traits<InputIterator>::value_type;
      if constexpr (std::is_trivial_v<T>)
        return OutputIterator(std::memcpy(d_first, first, (last - first) * sizeof(T)));
      else if constexpr (std::is_nothrow_move_constructible_v<T>)
        return std::uninitialized_move(first, last, d_first);
      else
        return std::uninitialized_copy(first, last, d_first);
    }

    template<typename InputIterator, typename OutputIterator>
    OutputIterator move_range_optimal(InputIterator first, InputIterator last, OutputIterator d_first)
    {
      using T = typename std::iterator_traits<InputIterator>::value_type;
      if constexpr (std::is_trivial_v<T>)
        return OutputIterator(std::memcpy(d_first, first, (last - first) * sizeof(T)));
      else if constexpr (std::is_nothrow_move_constructible_v<T>)
        return std::move(first, last, d_first);
      else
        return std::copy(first, last, d_first);
    }

    template<typename InputIterator, typename OutputIterator>
    OutputIterator move_range_optimal_backward(InputIterator first, InputIterator last, OutputIterator d_first)
    {
      using T = typename std::iterator_traits<InputIterator>::value_type;
      if constexpr (std::is_trivial_v<T>)
        return OutputIterator(std::memmove(d_first, first, (last - first) * sizeof(T)));
      else if constexpr (std::is_nothrow_move_constructible_v<T>)
        return std::move_backward(first, last, d_first);
      else
        return std::copy_backward(first, last, d_first);
    }

    template<typename InputIterator, typename OutputIterator>
    OutputIterator copy_range_optimal(InputIterator first, InputIterator last, OutputIterator d_first)
    {
      using T = typename std::iterator_traits<InputIterator>::value_type;
      if constexpr (std::is_trivial_v<T>)
        return OutputIterator(std::memcpy(d_first, first, (last - first) * sizeof(T)));
      else
        return std::copy(first, last, d_first);
    }

    template<typename InputIterator, typename OutputIterator>
    OutputIterator uninitialized_copy_range_optimal(InputIterator first, InputIterator last, OutputIterator d_first)
    {
      using T = typename std::iterator_traits<InputIterator>::value_type;
      if constexpr (std::is_trivial_v<T>)
        return OutputIterator(std::memcpy(d_first, first, (last - first) * sizeof(T)));
      else
        return std::uninitialized_copy(first, last, d_first);
    }
  }
}