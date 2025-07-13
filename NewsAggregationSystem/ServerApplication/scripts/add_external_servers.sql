INSERT INTO external_servers (name, base_url, api_key, status, last_accessed) VALUES
('NewsAPI.org', 'https://newsapi.org', '8f4cda27870e4de79de71ec7876d3733', true, NOW()),
('TheNewsAPI.com', 'https://api.thenewsapi.com', 'PLFrdIv6ewEpzWbUrL2MXBn5Z6ZtD7AAuD5rUUhY', true, NOW())
ON DUPLICATE KEY UPDATE
status = VALUES(status),
last_accessed = NOW(); 