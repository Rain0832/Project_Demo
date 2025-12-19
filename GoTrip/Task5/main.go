package main

import (
	"encoding/json"
	"fmt"
)

type User struct {
	Name string `json:"name"`
	Age  int    `json:"age"`
}

func main() {
	// 1
	u1 := User{Name: "Tom", Age: 19}
	u2 := User{Name: "Amy", Age: 29}
	u3 := User{Name: "Tim", Age: 39}
	jsonBytes1, err1 := json.Marshal(u1)
	jsonBytes2, err2 := json.Marshal(u2)
	jsonBytes3, err3 := json.Marshal(u3)
	if err1 == nil && err2 == nil && err3 == nil {
		fmt.Println(string(jsonBytes1))
		fmt.Println(string(jsonBytes2))
		fmt.Println(string(jsonBytes3))
	}

	// 2
	u10 := User{}
	fmt.Println(u10)
	data := `{"name": "Alice", "age": 22}`
	err4 := json.Unmarshal([]byte(data), &u10)
	if err4 == nil {
		fmt.Println(u10)
	}

	// 3
	u4 := User{Name: "Tom4", Age: 19}
	u5 := User{Name: "Amy5", Age: 29}
	u6 := User{Name: "Tim6", Age: 39}
	users := []User{u4, u5, u6}
	jsonBytes5, err5 := json.Marshal(users[0])
	jsonBytes6, err6 := json.Marshal(users[1])
	jsonBytes7, err7 := json.Marshal(users[2])
	if err5 == nil && err6 == nil && err7 == nil {
		fmt.Println(string(jsonBytes5))
		fmt.Println(string(jsonBytes6))
		fmt.Println(string(jsonBytes7))
	}

	jsonBytes8, err8 := json.Marshal(users)
	if err8 == nil {
		fmt.Println(string(jsonBytes8))
	}
}
