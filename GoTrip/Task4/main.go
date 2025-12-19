package main

import "fmt"

type User struct {
	Name string
	Age  int
}

func (u User) SayHi() {
	fmt.Println("Hi! I'm", u.Name)
	fmt.Println("I'm", u.Age)
}

func main() {
	// 1
	score := make(map[string]int)
	score["A"] = 100
	score["B"] = 70
	score["C"] = 90
	score["D"] = 80
	fmt.Println("A = ", score["A"])
	fmt.Println("B = ", score["B"])
	score["A"] = 99
	fmt.Println("A = ", score["A"])
	delete(score, "A")
	fmt.Println("A = ", score["A"])
	for k, v := range score {
		fmt.Println(k, "=", v)
	}

	// 2
	user1 := User{"Tom", 20}
	user2 := User{Name: "Amy", Age: 19}
	user3 := &User{"Daming", 27}
	fmt.Println(user1)
	fmt.Println(user2)
	fmt.Println(user3)
	user1.SayHi()
	user2.SayHi()
	user3.SayHi()
	users := map[int]User{1: user1, 2: user2, 3: *user3}
	for k, v := range users {
		fmt.Println(k, v)
		v.SayHi()
	}
}
