package main

import (
	"encoding/json"
	"log"
	"net/http"
	"os"
	"sync"
)

type FileUserStorage struct {
	filePath string
	mu       sync.Mutex
	users    []User
}

func NewFileUserStorage(path string) (*FileUserStorage, error) {
	s := &FileUserStorage{
		filePath: path,
		users:    []User{},
	}

	bytes, err := os.ReadFile(s.filePath)
	if err != nil {
		if os.IsNotExist(err) {
			return s, nil
		}
		return nil, err
	}

	if err := json.Unmarshal(bytes, &s.users); err != nil {
		return nil, err
	}

	return s, nil

}

type UserStorage interface {
	ListUser() ([]User, error)
	AddUser(User) (User, error)
}

func (s *FileUserStorage) ListUser() ([]User, error) {
	s.mu.Lock()
	defer s.mu.Unlock()
	copyUser := make([]User, len(s.users))
	copy(copyUser, s.users)
	return copyUser, nil
}

func (s *FileUserStorage) AddUser(user User) (User, error) {
	s.mu.Lock()
	defer s.mu.Unlock()

	maxID := 0
	for _, u := range s.users {
		if u.ID > maxID {
			maxID = u.ID
		}
	}

	user.ID = maxID + 1
	newUsers := append(s.users, user)
	if err := s.saveUnlocked(newUsers); err != nil {
		log.Printf("保存用户失败: %v", err)
		return User{}, &AppError{
			Code:       2001,
			Message:    "保存用户失败",
			HTTPStatus: http.StatusInternalServerError, // 存储失败是服务端错，500
			Err:        err,
		}
	}

	s.users = newUsers
	log.Printf("新增用户成功: id = %d", user.ID)
	return user, nil
}

func (s *FileUserStorage) saveUnlocked(users []User) error {
	bytes, err := json.Marshal(users)
	if err != nil {
		return err
	}
	return os.WriteFile(s.filePath, bytes, 0644)
}

func (s *FileUserStorage) Close() error {
	log.Printf("storage resource freed")
	return nil
}
