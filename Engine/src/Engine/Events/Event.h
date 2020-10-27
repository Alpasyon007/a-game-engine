#pragma once

#include "enginepch.h"
#include "Engine/Core.h"

namespace Engine {

	// Events are currently blocking, meaning when an event occurs it
	// immediately gets dispatched and must be dealt with right then an there.
	// For future, a better strategy might be to buffer events in an event
	// bus and process them during the "event" part of the update stage.

	enum class EventType {
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCatgeory {
		None = 0,
		EventCatgeoryApplication	= BIT(0),
		EventCategeoryInput			= BIT(1),
		EventCatgeoryKeyboard		= BIT(2),
		EventCatgeoryMouse			= BIT(3),
		EventCatgeoryMouseButton	= BIT(4)
	};

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type;}\
								virtual EventType GetEventType() const override { return GetStaticType();}\
								virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCatgeoryFlags() const override { return category; }

	class ENGINE_API Event {
		friend class EventDispatcher;
	public:
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCatgeoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		inline bool IsInCatgeory(EventCatgeory catgeory) {
			return GetCatgeoryFlags() & catgeory;
		}
	protected:
		bool m_Handled = false;
	};

	class EventDispatcher {
		template<typename T>
		using EventFn = std::function<bool(T&)>;
	public:
		EventDispatcher(Event& event)
			: m_Event(event){
		}

		template<typename T>
		bool Dispatch(EventFn<T> func) {
			if (m_Event.GetEventType() == T::GetStaticTyep()) {
				m_Event.m_Handled = func(*(T*)&m_Event);
				return true;
			}
			return fales;
		}
	private:
		Event& m_Event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e) {
		return os << e.ToString();
	}
}