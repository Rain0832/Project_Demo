package main

import (
	"errors"
	"fmt"
)

func add(a int, b int) int {
	return a + b
}

func divide(a int, b int) (int, error) {
	if b == 0 {
		return 0, errors.New("divide by zero")
	}
	return a / b, nil
}

func main() {
	x := add(10, 20)
	var result1, err1 = divide(60, 20)
	var result2, err2 = divide(10, 0)
	fmt.Println(x)
	if err1 != nil {
		fmt.Println("error1: ", err1)
	} else {
		fmt.Println("ok1: ", result1)
	}
	if err2 != nil {
		fmt.Println("error2: ", err2)
	} else {
		fmt.Println("ok2: ", result2)
	}
}
