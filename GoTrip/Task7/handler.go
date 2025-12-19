package main

import (
	"encoding/json"
	"log"
	"net/http"

	"github.com/go-playground/validator/v10"
)

type UserHandler struct {
	storage UserStorage
}

func NewUserHandler(storage UserStorage) *UserHandler {
	return &UserHandler{storage: storage}
}

func (h *UserHandler) GetUsers(w http.ResponseWriter, r *http.Request) {
	users, err := h.storage.ListUser()
	if err != nil {
		http.Error(w, "internal error", 500)
		return
	}

	w.Header().Set("Content-Type", "application/json")
	json.NewEncoder(w).Encode(users)
}

var validate = validator.New()

func (h *UserHandler) CreateUser(w http.ResponseWriter, r *http.Request) {
	var req CreateUserRequest
	if err := json.NewDecoder(r.Body).Decode(&req); err != nil {
		log.Printf("Decode request fail: %v", err)
		WriteError(w, &AppError{
			Code:       1003,
			Message:    "请求格式错误（请传入合法 JSON）",
			HTTPStatus: http.StatusBadRequest, // 明确 HTTP 状态码 400
			Err:        err,
		})
		return
	}

	if err := validate.Struct(req); err != nil {
		log.Printf("参数校验失败：%v", err)
		ErrInvalidParams.Err = err
		WriteError(w, ErrInvalidParams)
		return
	}

	// 3. 存储错误：返回 500（服务端内部错）
	user := User{Name: req.Name, Age: req.Age}

	savedUser, err := h.storage.AddUser(user)
	if err != nil {
		log.Printf("创建用户失败：%v", err)
		// 判断是否是 AppError，不是则包装成 500
		if appErr, ok := err.(*AppError); ok {
			WriteError(w, appErr)
		} else {
			ErrInternal.Err = err
			WriteError(w, ErrInternal) // 自带 HTTP 500
		}
		return
	}

	// 4. 成功响应（合法 HTTP 状态码 201）
	w.Header().Set("Content-Type", "application/json; charset=utf-8")
	w.WriteHeader(http.StatusCreated) // 201 表示资源创建成功
	_ = json.NewEncoder(w).Encode(map[string]interface{}{
		"code":    0, // 业务码 0 表示成功
		"message": "创建成功",
		"user":    savedUser,
	})
}
