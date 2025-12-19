package main

import "fmt"

func modify(s []int) {
	s[0] = 999
}

func main() {
	s1 := []int{1, 2, 3}
	s2 := make([]int, 2, 5)
	a := [5]int{10, 20, 30, 40, 50}
	s3 := a[1:4]
	fmt.Println(s1, len(s1), cap(s1))
	fmt.Println(s2, len(s2), cap(s2))
	fmt.Println(s3, len(s3), cap(s3))

	s := make([]int, 0, 2)
	fmt.Println(s, len(s), cap(s))
	s = append(s, 1)
	fmt.Println(s, len(s), cap(s))
	s = append(s, 2)
	fmt.Println(s, len(s), cap(s))
	// double expense?
	s = append(s, 3)
	fmt.Println(s, len(s), cap(s))

	x := []int{1, 2, 3}
	fmt.Println(x, len(x), cap(x))
	modify(x)
	fmt.Println(x, len(x), cap(x))
}
