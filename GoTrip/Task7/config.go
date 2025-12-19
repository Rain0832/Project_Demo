package main

import (
	"encoding/json"
	"os"
)

type Config struct {
	ServerPort  string `json:"server_port"`
	StoragePath string `json:"storage_path"`
}

func LoadConfig(path string) (*Config, error) {
	data, err := os.ReadFile(path)
	if err != nil {
		return nil, err
	}

	var cfg Config
	if err := json.Unmarshal(data, &cfg); err != nil {
		return nil, err
	}

	if cfg.ServerPort == "" {
		cfg.ServerPort = ":8080"
	}
	if cfg.StoragePath == "" {
		cfg.StoragePath = "user.json"
	}

	return &cfg, nil
}
