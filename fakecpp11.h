#ifndef FAKECPP11_H
#define FAKECPP11_H


namespace std {

template<class ForwardIterator, class T>
void iota(ForwardIterator first, ForwardIterator last, T value)
{
	while(first != last) {
		*first++ = value;
		++value;
	}
}

}

#endif // FAKECPP11_H
