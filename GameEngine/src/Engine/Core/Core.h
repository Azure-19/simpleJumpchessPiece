#pragma once

#include <memory>			// mainly for safety

// Platform detection using predefined macros
#ifdef _WIN32
	/* Windows x64/x86 */
	#ifdef _WIN64
		/* Windows x64  */
		#define EG_PLATFORM_WINDOWS
		//#include <Windows.h> is invalid i don't know why
	#else
		/* Windows x86 */
		#error "x86 Builds are not supported!"
	#endif
#elif defined(__APPLE__) || defined(__MACH__)
	#include <TargetConditionals.h>
	/* TARGET_OS_MAC exists on all the platforms
	 * so we must check all of them (in this order)
	 * to ensure that we're running on MAC
	 * and not some other Apple platform */
	#if TARGET_IPHONE_SIMULATOR == 1
		#error "IOS simulator is not supported!"
	#elif TARGET_OS_IPHONE == 1
		#define EG_PLATFORM_IOS
		#error "IOS is not supported!"
	#elif TARGET_OS_MAC == 1
		#define EG_PLATFORM_MACOS
		#error "MacOS is not supported!"
	#else
		#error "Unknown Apple platform!"
	#endif
 /* We also have to check __ANDROID__ before __linux__
  * since android is based on the linux kernel
  * it has __linux__ defined */
#elif defined(__ANDROID__)
	#define EG_PLATFORM_ANDROID
	#error "Android is not supported!"
#elif defined(__linux__)
	#define EG_PLATFORM_LINUX
	#error "Linux is not supported!"
#else
	/* Unknown compiler/platform */
	#error "Unknown platform!"
#endif // End of platform detection


#ifdef EG_DEBUG
	#if defined(EG_PLATFORM_WINDOWS)
		#define EG_DEBUGBREAK() __debugbreak()
	#elif defined(EG_PLATFORM_LINUX)
		#include <signal.h>
		#define EG_DEBUGBREAK() raise(SIGTRAP)
	#else
		#error "Platform doesn't support debugbreak yet!"
	#endif
	#define EG_ENABLE_ASSERTS
#else
	#define EG_DEBUGBREAK()
#endif

#ifdef EG_DEBUG
	#define EG_ENABLE_ASSERTS
#endif

// TODO: Make this macro able to take in no arguments except condition
#ifdef EG_ENABLE_ASSERTS
	#define EG_ASSERT(x, ...)      { if(!(x)) { EG_ERROR("Assertion Failed: {0}", __VA_ARGS__); EG_DEBUGBREAK(); } }
	#define EG_CORE_ASSERT(x, ...) { if(!(x)) { EG_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); EG_DEBUGBREAK(); } }
#else
	#define EG_ASSERT(x, ...)
	#define EG_CORE_ASSERT(x, ...)
#endif


#define BIT(x) (1 << x)

#define EG_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Engine {
	// using not only declare that it's on use but also enable a template
	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	// just make our life easier| Ref<Shader> ->  Engine::Ref<Shader>

}