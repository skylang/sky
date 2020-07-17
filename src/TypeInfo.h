// Copyright (c) 2018 Stephan Unverwerth
// This code is licensed under MIT license (See LICENSE for details)

#ifndef TypeInfo_h
#define TypeInfo_h

#include <map>

#define SKY_GET_TYPE(CLS, PAR) \
		const TypeInfo* getTypeInfo() const override { return CLS::getStaticTypeInfo(); } \
		static const TypeInfo* getStaticTypeInfo() { static TypeInfo dummy(#CLS, PAR::getStaticTypeInfo()); return &dummy; }

#define SKY_BASE_TYPE(CLS) \
		virtual const TypeInfo* getTypeInfo() const { return CLS::getStaticTypeInfo(); } \
		template<typename T> T* as() { return getTypeInfo()->extendsOrEquals<T>() ? static_cast<T*>(this) : nullptr; } \
		template<typename T> const T* as() const { return getTypeInfo()->extendsOrEquals<T>() ? static_cast<const T*>(this) : nullptr; } \
		static const TypeInfo* getStaticTypeInfo() { static TypeInfo dummy(#CLS, nullptr); return &dummy; }

namespace Sky {

	struct TypeInfo {
		TypeInfo(const char* name, const TypeInfo* parent): name(name), parent(parent) {}

		bool extends(const TypeInfo* tid) const;

		template<typename T> bool extends() const {
			return extends(T::getStaticTypeInfo());
		}

		template<typename T> bool extendsOrEquals() const {
            const auto ti = T::getStaticTypeInfo();
			return ti == this || extends(ti);
		}

		template<typename T> bool is() const {
			return T::getStaticTypeInfo() == this;
		}

        const char* getName() const { return name; }
        const TypeInfo* getParent() const { return parent; }

	private:
		mutable std::map<const TypeInfo*, bool> extCache;
		const char* name;
		const TypeInfo* parent;
	};
}


#endif