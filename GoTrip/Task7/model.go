package main

type User struct {
	ID   int    `json:"id"`
	Name string `json:"name"`
	Age  int    `json:"age"`
}

type CreateUserRequest struct {
	Name string `json:"name" validate:"required,min=2,max=20"`
	Age  int    `json:"age" validate:"gte=0,lte=150"`
}
