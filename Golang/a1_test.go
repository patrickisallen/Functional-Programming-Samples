package main

import (
	// "errors"
	"fmt"
	"testing"
)

func TestStack(t *testing.T) {
	stackSliceTest()
	stackLinkedTest()
}

func stackerTest(s Stacker) {
	initStackL := makeStackLinked()

	if initStackL.isEmpty() != true {
		panic("isEmpty() FAILED when stack is empty")
	}

	if initStackL.size() != 0 {
		panic("size() FAILED when stack is empty")
	}

	popValLinked, _ := initStackL.pop()

	if popValLinked != 0 {
		panic("pop() FAILED when stack is empty")
	}

	peekValLinked, _ := initStackL.peek()

	if peekValLinked != 0 {
		panic("peek() FAILED when stack is empty")
	}

	initStackL.push(4)

	if initStackL.isEmpty() != false {
		panic("isEmpty() FAILED when stack is 1")
	}

	if initStackL.size() != 1 {
		panic("size() FAILED when stack is 1")
	}

	peekValLinked, _ = initStackL.peek()

	if peekValLinked != 4 {
		panic("peek() FAILED and returned wrong value")
	}

	popValLinked, _ = initStackL.pop()

	if popValLinked != 4 {
		panic("pop() FAILED and returned wrong value when stack is 1")
	}

	initStackL.push(5)
	initStackL.push(6)
	initStackL.push(7)
	initStackL.push(8)
	initStackL.push(9)
	initStackL.push(10)

	if initStackL.size() != 6 {
		panic("push() & size() FAILED when values [5,6,7,8,9,10] are pushed onto an empty stack, and size is should be 6")
	}

	stackLinkedCopy := initStackL.copy()

	if stackEquals(stackLinkedCopy, initStackL) != true {
		panic("copy() FAILED when copying a stack with 6 elements, stack's should be EQUAL when calling stackEquals()")
	}

	peekValLinked, _ = stackLinkedCopy.peek()

	if peekValLinked != 10 {
		panic("peek() FAILED on stack whose top element is value=10")
	}

	popAll(stackLinkedCopy)

	if stackLinkedCopy.isEmpty() != true {
		panic("popAll() FAILED on a stack with 6 elements, stack should be empty")
	}

	if stackLinkedCopy.size() != 0 {
		panic("Stack should be size 0 when performing popAll() on stack with 6 elements initially")
	}
}

func stackLinkedTest() {
	s := makeStackLinked()
	stackerTest(s)
	fmt.Println("All tests finished for stackLinkedTest()")
}

func stackSliceTest() {
	s := makeStackSlice()
	stackerTest(s)
	fmt.Println("All tests finished for stackSliceTest")
}
