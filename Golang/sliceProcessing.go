package main

import (
	"errors"
	"fmt"
	"strconv"
)

//type Stacker interface {
// Pre-condition:
//    none
// Post-condition:
//    returns true if this stack has no elements, and false otherwise
// Performance:
//    runs in O(1) time
//isEmpty() bool

// Pre-condition:
//    none
// Post-condition:
//    returns the number of elements in this stack
// Performance:
//    runs in O(1) time
//size() int

// puts x on the top of the stack (increasing size by 1)
// Pre-condition:
//    none
// Post-condition:
//    puts a copy of x on top of the stack (and the size of the stack
//    increases by 1)
// Performance:
//    runs in O(1) time most of the time; can occasionally run in O(n)
//    time if memory re-organization is needed
//push(x int)

// Pre-condition:
//    none
// Post-condition:
//    If the stack is empty, returns 0 and a non-nil error value (with a
//    helpful error message).
//    If the stack is not empty, returns a copy of the top element of the
//    stack, and a nil value for the error.
//    In both cases, the stack has the same value after value after calling
//    peek() as before.
// Performance:
//    runs in O(1) time
//peek() (int, error)

// Pre-condition:
//    none
// Post-condition:
//    If the stack is empty, returns 0 and a non-nil error value (with a
//    helpful error message). The stack remains empty.
//    If the stack is not empty, returns, and removes, the top element of
//    the stack, and a nil value for the error. In this case, the size of
//    the stack decreases by 1.

// Performance:
//    runs in O(1) time
//pop() (int, error)

// Pre-condition:
//    none
// Post-condition:
//    Returns a copy of this stack.
// Performance:
//    runs in O(n) time, where n is the size of the stack
//copy() Stacker
//} // Stacker

type Stacker interface {
	isEmpty() bool
	push(x int)
	size() int
	pop() (int, error)
	peek() (int, error)
	copy() Stacker
}

type StackSlice struct {
	s []int
}

/* Structs for StackLinked */

type StackLinked struct {
	stackSize int
	head      *StackElement
	tail      *StackElement
}

type StackElement struct {
	value  int
	parent *StackElement
	child  *StackElement
}

// ------------------------- Methods for StackSlice struct ------------------//

func (s *StackSlice) isEmpty() bool {
	if len(s.s) == 0 {
		return true
	} else {
		return false
	}
}

func (s *StackSlice) size() int {
	e := len(s.s)
	return e

}

func (s *StackSlice) push(x int) {
	s.s = append(s.s, x)
}

func (s *StackSlice) pop() (int, error) {
	if len(s.s) == 0 {
		return 0, errors.New("Stack is empty, unable to perform pop()")
	} else {
		t := s.s[len(s.s)-1]
		s.s = s.s[:len(s.s)-1]
		return t, errors.New("No Errors")
	}
}

func (s *StackSlice) peek() (int, error) {
	if len(s.s) == 0 {
		return 0, errors.New("Stack is empty, unable to perform peek()")
	} else {
		t := s.s[len(s.s)-1]
		return t, errors.New("No Errors")
	}
}

func (s *StackSlice) copy() Stacker {
	tmp := &StackSlice{s: make([]int, len(s.s))}
	copy(tmp.s, s.s)
	return tmp
}

// ------------------------- Methods for StackLinked struct ------------------//

func (s *StackLinked) push(x int) {
	if s.stackSize > 0 {
		tmp := StackElement{value: x}
		tmp.parent = s.tail
		s.tail.child = &tmp
		s.tail = &tmp

		s.stackSize++
	} else {
		tmp := StackElement{value: x}
		s.head = &tmp
		s.tail = s.head
		s.stackSize++
	}
	//print(s)
}

func (s *StackLinked) isEmpty() bool {
	if s.stackSize == 0 {
		return true
	} else {
		return false
	}
}

func (s *StackLinked) size() int {
	return s.stackSize

}

func (s *StackLinked) pop() (int, error) {
	if s.isEmpty() == true {
		return 0, errors.New("Stack is empty, unable to perform pop()")
	} else if s.size() == 1 {
		tmp := s.tail.value
		s.tail = nil
		s.head = nil
		s.stackSize--
		return tmp, errors.New("No errors")
	} else {
		tmp := s.tail.value
		s.tail = s.tail.parent
		s.tail.child = nil
		s.stackSize--
		return tmp, errors.New("No Errors")
	}
}

func (s *StackLinked) peek() (int, error) {
	if s.isEmpty() == true {
		return 0, errors.New("Stack empty")
	} else {
		tmp := s.tail.value
		return tmp, errors.New("No errors")
	}
}

func (s *StackLinked) copy() Stacker {
	copiedStack := makeStackLinked()
	i := 0
	tmp := s.head
	for i < s.size() && tmp != nil {
		copiedStack.push(tmp.value)
		tmp = tmp.child
	}
	return copiedStack
}

// ------------- Methods to Create StackSlice & StackLinked ----------//

func makeStackSlice() Stacker {
	s := &StackSlice{s: []int{}}
	return s
}

func makeStackLinked() Stacker {
	s := &StackLinked{
		stackSize: 0,
	}
	return s
}

// ------------- Helper Function to print linkedlist stack with pointers -------//

func print(s *StackLinked) {
	i := 0
	tmp := s.head
	for i < s.stackSize && tmp != nil {
		fmt.Println(tmp)
		tmp = tmp.child
	}
	fmt.Println("--------End of line-------")
}

// ------------------- Stringer implementation to printout StackSlice struct & StackLinked struct ------//

func (s *StackSlice) String() string {
	tmp := ""
	i := 0
	for i < s.size() {
		if i == s.size()-1 {
			tmp += strconv.Itoa(s.s[i])
			i++
		} else {
			tmp += strconv.Itoa(s.s[i]) + "," + " "
			i++
		}

	}
	return "StackSlice: [" + tmp + "]"
}

func (s *StackLinked) String() string {
	theStack := ""
	i := 0
	tmp := s.head
	for i < s.stackSize && tmp != nil {
		if i == s.size()-1 {
			theStack += strconv.Itoa(tmp.value)
			tmp = tmp.child
			i++
		} else {
			theStack += strconv.Itoa(tmp.value) + "," + " "
			tmp = tmp.child
			i++
		}
	}
	return "StackLinked: [" + theStack + "]"
}

// -------------------- Additional Methods popAll() & stackEquals() -------------//

func popAll(s Stacker) {
	if s.isEmpty() == true {
		fmt.Println("Stack is empty, cannot perform popAll()")
	} else {
		i := 0
		sizeStack := s.size()
		for i < sizeStack {
			s.pop()
			i++
		}
	}
}

func stackEquals(s, t Stacker) bool {

	sCopy := s.copy()
	tCopy := t.copy()

	if sCopy.size() != tCopy.size() {
		fmt.Println("Stack Sizes do not much")
		return false
	} else {
		i := 0
		sValue := 0
		tValue := 0
		sizeOfStack := sCopy.size()
		for i < sizeOfStack {

			sValue, _ = sCopy.pop()
			tValue, _ = tCopy.pop()
			if sValue != tValue {
				return false
			}
			i++
		}
		return true
	}
}

// -------------------------------------------------------------------------------//

func main() {

}

/*
Citation:
https://stackoverflow.com/questions/18042439/go-append-to-slice-in-struct
https://stackoverflow.com/questions/45652560/interfaces-and-pointer-receivers
https://tour.golang.org/methods/4
https://stackoverflow.com/questions/30182538/why-can-not-i-duplicate-a-slice-with-copy-in-golang
*/

/* TESTING FUNCTIONS PLACED IN a1_test.go IGNORE THIS


func stackerTest(s Stacker) {
	pass := 0
	if s.isEmpty() != true {
		fmt.Println("isEmpty() FAILED when stack is empty")
	} else {
		pass++
	}

	if s.size() != 0 {
		fmt.Println("size() FAILED when stack is empty")
	} else {
		pass++
	}

	popVal, _ := s.pop()

	if popVal != 0 {
		fmt.Println("pop() FAILED when stack is empty")
	} else {
		pass++
	}

	peekVal, _ := s.peek()

	if peekVal != 0 {
		fmt.Println("peek() FAILED when stack is empty")
	} else {
		pass++
	}

	s.push(4)

	if s.isEmpty() != false {
		fmt.Println("isEmpty() FAILED when stack is 1")
	} else {
		pass++
	}

	if s.size() != 1 {
		fmt.Println("size() FAILED when stack is 1")
	} else {
		pass++
	}

	peekVal, _ = s.peek()

	if peekVal != 4 {
		fmt.Println("peek() FAILED and returned wrong value")
	} else {
		pass++
	}

	popVal, _ = s.pop()

	if popVal != 4 {
		fmt.Println("pop() FAILED and returned wrong value when stack is 1")
	} else {
		pass++
	}

	s.push(5)
	s.push(6)
	s.push(7)

	if pass == 8 {
		fmt.Println("All tests passed")
	} else {
		fmt.Println("FAILURE detected")
	}

}

func stackSliceTest() {
	s := makeStackSlice()
	stackerTest(s)
	//fmt.Println("all tests passed")
}

func stackLinkedTest() {
	s := makeStackLinked()
	stackerTest(s)
	//fmt.Println("all tests passed")
}
*/
