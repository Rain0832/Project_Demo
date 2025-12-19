package main

import (
	"encoding/json"
	"net/http"
)

type AppError struct {
	Code       int    `json:"code"`
	Message    string `json:"message"`
	HTTPStatus int    `json:"-"`
	Err        error  `json:"-"`
}

func (e *AppError) Error() string {
	if e.Err != nil {
		return e.Err.Error()
	}
	return e.Message
}

var (
	ErrUserNotFound  = &AppError{Code: 1001, Message: "User not exist", HTTPStatus: http.StatusNotFound, Err: nil}
	ErrInvalidParams = &AppError{Code: 1002, Message: "Invalid params", HTTPStatus: http.StatusBadRequest, Err: nil}
	ErrInternal      = &AppError{Code: 5000, Message: "Server internal error", HTTPStatus: http.StatusInternalServerError, Err: nil}
	ErrBadRequest    = &AppError{Code: http.StatusBadRequest, Message: "Request type error", HTTPStatus: http.StatusBadRequest, Err: nil}
)

func WriteError(w http.ResponseWriter, err *AppError) {
	httpStatus := err.HTTPStatus
	if httpStatus == 0 {
		httpStatus = http.StatusInternalServerError
	}

	w.Header().Set("Content-Type", "application/json; charset=utf-8")
	w.WriteHeader(httpStatus)

	resp := map[string]interface{}{
		"code":    err.Code,
		"message": err.Message,
	}

	if encodeErr := json.NewEncoder(w).Encode(resp); encodeErr != nil {
		_, _ = w.Write([]byte(err.Message))
	}
}
