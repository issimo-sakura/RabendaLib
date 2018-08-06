//
// Created by Rabenda on 09/07/2017.
//

#ifndef RABENDALIB_STL_UNINITIALIZED_HPP
#define RABENDALIB_STL_UNINITIALIZED_HPP

#include <rabenda/stl/stl_config.hpp>
#include <rabenda/stl/stl_iterator_base.hpp>
#include <rabenda/stl/type_traits.hpp>
#include <rabenda/stl/stl_algobase.hpp>
#include <rabenda/stl/stl_construct.hpp>

NAMESPACE_BEGIN(stl)
	//!uninitialized_copy
	template<class _InputIter, class _ForwardIter>
	inline _ForwardIter
	__uninitialized_copy_aux(_InputIter __first, _InputIter __last, _ForwardIter __result, __true_type) {
		return copy(__first, __last, __result);
	}
	
	template<class _InputIter, class _ForwardIter>
	inline _ForwardIter
	__uninitialized_copy_aux(_InputIter __first, _InputIter __last, _ForwardIter __result, __false_type) {
		_ForwardIter __cur = __result;
		__STL_TRY {
			for (; __first != __last; ++__first, ++__cur)
				_Construct(&*__cur, *__first);
			return __cur;
		}
		__STL_UNWIND(_Destroy(__result, __cur));
	}
	
	template<class _InputIter, class _ForwardIter, class _Tp>
	inline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, _Tp *) {
		using _Is_POD = typename __type_traits<_Tp>::is_POD_type;
		return __uninitialized_copy_aux(__first, __last, __result, _Is_POD());
	}
	
	
	template<class _InputIter, class _ForwardIter>
	inline _ForwardIter uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result) {
		return __uninitialized_copy(__first, __last, __result, __VALUE_TYPE(__first));
	}
	
	inline char *uninitialized_copy(const char *__first, const char *__last, char *__result) {
		memmove(__result, __first, __last - __first);
		return __result + (__first - __last);
	}
	
	inline wchar_t *uninitialized_copy(const wchar_t *__first, const wchar_t *__last, wchar_t *__result) {
		memmove(__result, __first, sizeof(wchar_t) * (__last - __first));
		return __result + (__first - __last);
	}
	
	//!uninitialized_copy_n
	
	template<class _InputIter, class _Size, class _ForwardIter>
	inline pair<_InputIter, _ForwardIter>
	__uninitialized_copy_n(_InputIter __first, _Size __count, _ForwardIter __result, input_iterator_tag) {
		_ForwardIter __cur = __result;
		__STL_TRY {
			for (; __count > 0; --__count, ++__first, ++__cur)
				_Construct(&*__cur, *__first);
			return pair<_InputIter, _ForwardIter>(__first, __cur);
		}
		__STL_UNWIND(_Destroy(__result, __cur));
	}
	
	template<class _RandomAccessIter, class _Size, class _ForwardIter>
	inline pair<_RandomAccessIter, _ForwardIter>
	__uninitialized_copy_n(_RandomAccessIter __first, _Size __count, _ForwardIter __result,
	                       random_access_iterator_tag) {
		_RandomAccessIter __last = __first + __count;
		return pair<_RandomAccessIter, _ForwardIter>(__last, uninitialized_copy(__first, __last, __result));
	}
	
	template<class _InputIter, class _Size, class _ForwardIter>
	inline pair<_InputIter, _ForwardIter>
	__uninitialized_copy_n(_InputIter __first, _Size __count, _ForwardIter __result) {
		return __uninitialized_copy_n(__first, __count, __result, __ITERATOR_CATEGORY(__first));
	}
	
	template<class _InputIter, class _Size, class _ForwardIter>
	inline pair<_InputIter, _ForwardIter>
	uninitialized_copy_n(_InputIter __first, _Size __count, _ForwardIter __result) {
		return __uninitialized_copy_n(__first, __count, __result, __ITERATOR_CATEGORY(__first));
	}
	
	//!uninitialized_fill
	template<class _ForwardIter, class _Tp>
	inline void __uninitialized_fill_aux(_ForwardIter __first, _ForwardIter __last, const _Tp &__x, __false_type) {
		_ForwardIter __cur = __first;
		__STL_TRY {
			for (; __cur != __last; ++__cur)
				_Construct(&*__cur, __x);
		}
		__STL_UNWIND(_Destroy(__first, __cur));
	}
	
	template<class _ForwardIter, class _Tp>
	inline void __uninitialized_fill_aux(_ForwardIter __first, _ForwardIter __last, const _Tp &__x, __true_type) {
		fill(__first, __last, __x);
	}
	
	
	template<class _ForwardIter, class _Tp, class _Tp1>
	inline void __uninitialized_fill(_ForwardIter __first, _ForwardIter __last, const _Tp &__x, _Tp1 *) {
		using _Is_POD = typename __type_traits<_Tp1>::is_POD_type;
		__uninitialized_fill_aux(__first, __last, __x, _Is_POD());
	}
	
	template<class _ForwardIter, class _Tp>
	inline void uninitialized_fill(_ForwardIter __first, _ForwardIter __last, const _Tp &__x) {
		__uninitialized_fill(__first, __last, __x, __VALUE_TYPE(__first));
	}
	
	//!uninitialized_fill_n
	template<class _ForwardIter, class _Size, class _Tp>
	inline _ForwardIter __uninitialized_fill_n_aux(_ForwardIter __first, _Size __count, const _Tp &__x, __false_type) {
		_ForwardIter __cur = __first;
		__STL_TRY {
			for (; __count > 0; --__count, ++__cur)
				_Construct(&*__cur, __x);
			return __cur;
		}
		__STL_UNWIND(_Destroy(__first, __cur));
	}
	
	template<class _ForwardIter, class _Size, class _Tp>
	inline _ForwardIter __uninitialized_fill_n_aux(_ForwardIter __first, _Size __count, const _Tp &__x, __true_type) {
		return fill_n(__first, __count, __x);
	}
	
	template<class _ForwardIter, class _Size, class _Tp, class _Tp1>
	inline _ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __count, const _Tp &__x, _Tp1 *) {
		using _Is_POD = typename __type_traits<_Tp1>::is_POD_type;
		return __uninitialized_fill_n_aux(__first, __count, __x, _Is_POD());
	}
	
	template<class _ForwardIter, class _Size, class _Tp>
	inline _ForwardIter uninitialized_fill_n(_ForwardIter __first, _Size __count, const _Tp &__x) {
		return __uninitialized_fill_n(__first, __count, __x, __VALUE_TYPE(__first));
	}
	
	//! Extensions: __uninitialized_copy_copy
	//! Copies [first1, last1) into [result, result + (last1 - first1)), and
	//!  copies [first2, last2) into
	//!  [result, result + (last1 - first1) + (last2 - first2)).
	template<class _InputIter1, class _InputIter2, class _ForwardIter>
	inline _ForwardIter
	__uninitialized_copy_copy(_InputIter1 __first1, _InputIter1 __last1, _InputIter2 __first2, _InputIter2 __last2,
	                          _ForwardIter __result) {
		_ForwardIter __mid = uninitialized_copy(__first1, __last1, __result);
		__STL_TRY {
			return uninitialized_copy(__first2, __last2, __mid);
		}
		__STL_UNWIND(_Destroy(__result, __mid));
	}
	
	//! Extensions: __uninitialized_fill_copy
	//! Fills [result, mid) with x, and copies [first, last) into
	//!  [mid, mid + (last - first)).
	template<class _ForwardIter, class _Tp, class _InputIter>
	inline _ForwardIter
	__uninitialized_fill_copy(_ForwardIter __result, _ForwardIter __mid, const _Tp &__x, _InputIter __first,
	                          _InputIter __last) {
		uninitialized_fill(__result, __mid, __x);
		__STL_TRY {
			return uninitialized_copy(__first, __last, __mid);
		}
		__STL_UNWIND(_Destroy(__result, __mid));
	}
	
	//! Extensions: __uninitialized_copy_fill
	//! Copies [first1, last1) into [first2, first2 + (last1 - first1)), and
	//!  fills [first2 + (last1 - first1), last2) with x.
	template<class _InputIter, class _ForwardIter, class _Tp>
	inline void
	__uninitialized_copy_fill(_InputIter __first1, _InputIter __last1, _ForwardIter __first2, _ForwardIter __last2,
	                          const _Tp &__x) {
		_ForwardIter __mid = uninitialized_copy(__first1, __last1, __first2);
		__STL_TRY {
			uninitialized_fill(__mid, __last2, __x);
		}
		__STL_UNWIND(_Destroy(__first2, __mid));
	}
NAMESPACE_END(stl)

#endif //RABENDALIB_STL_UNINITIALIZED_HPP
