/********************************************************************
* Name          : sequencer.h
* Description   : Basic functional style programming in modern C++
* Author        : K.M.Hussain
* Licence       : Educational
********************************************************************/

#ifndef SEQUENCER_H
#define SEQUENCER_H

#include <iterator>
#include <functional>
#include <utility>

namespace Sequencer
{
	template<typename ElementType>
	class Sequence
	{
	public:
		using Enumerator = typename std::function<bool(ElementType&)>;

		Sequence(const Enumerator& e) : enumerate(e){}

		void Perform(std::function<void(const ElementType&)> action) const
		{
			ElementType entry; 

			while(enumerate(entry))
				action(entry);	
		}

		ElementType Combine(const ElementType& initial, std::function<ElementType(const ElementType&, const ElementType&)> combine) const
		{
			ElementType entry, result = initial;

			while(enumerate(entry))
				result = combine(result, entry);

			return result;
		}

		Sequence<ElementType> Choose(std::function<bool(const ElementType&)> allowed) const
		{
			return Sequence<ElementType>([allowed, this](ElementType& entry)
			{
				while(enumerate(entry))
					if(allowed(entry)) return true;
				return false;
			});
		}

		Sequence<ElementType> Limit(int count) const
		{
			return Sequence<ElementType>([count, this](ElementType& entry) mutable
			{
				return count-- > 0 && enumerate(entry);
			});
		}

		template<typename ResultType>
		Sequence<ResultType> TransformTo(std::function<ResultType(const ElementType&)> transformer) const
		{
			return Sequence<ResultType>([transformer, this](ResultType& entry)
			{
				ElementType value;
				if(enumerate(value))
					return entry = transformer(value), true;
				return false;
			});
		}

		template<typename UnaryOperation>
		auto Transform(UnaryOperation transformer) const -> Sequence<decltype(transformer(std::declval<ElementType>()))>
		{
			return TransformTo<decltype(transformer(std::declval<ElementType>()))>(transformer);
		}

	private:
		Enumerator enumerate;
	};

	template<typename InputIterator>
	Sequence<typename std::iterator_traits<InputIterator>::value_type> Fetch(InputIterator begin, InputIterator end)
	{
		using ElementType = typename std::iterator_traits<InputIterator>::value_type;

		return Sequence<ElementType>([begin, end](ElementType& entry) mutable
		{	
			if(begin != end)
			{
				entry = *begin;
				return ++begin, true;
			}
			return false;
		});
	}

	template<typename ContainerType>
	auto Fetch(const ContainerType& container) -> decltype(Fetch(std::begin(container), std::end(container)))
	{
		return Fetch(std::begin(container), std::end(container));
	}

	template<typename ElementType>
	class Enumerator
	{
	public:
		virtual bool GetCurrent(ElementType&) = 0;
		virtual ~Enumerator(){}
	};

	template<typename ElementType>
	Sequence<ElementType> Fetch(Enumerator<ElementType>&& source)
	{
		return Sequence<ElementType>([&source](ElementType& entry){return source.GetCurrent(entry);});
	}
}

#endif

