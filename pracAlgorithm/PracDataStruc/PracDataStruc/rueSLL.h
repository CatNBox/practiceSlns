#pragma once
#include <iostream>


template <typename type>
class sllNode
{
public:
	type value;
	sllNode<type>* nextNode;
};

template <typename type>
class rueSLL
{
private:
	sllNode<type>* head;
public:
};