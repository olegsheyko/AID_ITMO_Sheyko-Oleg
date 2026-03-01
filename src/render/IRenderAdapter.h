#pragma once
#include <string>

class IRenderAdapter {
	public:
	virtual ~IRenderAdapter() = default;

	// Создание окна и инициализация рендерера
	virtual bool init(int width, int height, const std::string& title) = 0;

	// Проверка, открыто ли окно
	virtual bool isRunning() const = 0;

	// Обработка событий
	virtual void pollEvents() = 0;

	// Очистка экрана и подготовка к рендерингу
	virtual void beginFrame(float r, float g, float b) = 0;

	// Завершение рендеринга и отображение на экране
	virtual void endFrame() = 0;

	// Закрытие окна и освобождение ресурсов
	virtual void shutdown() = 0;
};