//
// Copyright (c) 2010-2011 Matthew Jack and Doug Binks
//
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#pragma once

#ifndef OBJECTINTERFACE_INCLUDED
#define OBJECTINTERFACE_INCLUDED

#include <vector>

struct SystemTable;
struct IObject;


const size_t InvalidId = -1;
typedef size_t PerTypeObjectId;
typedef size_t ConstructorId;

struct ObjectId
{
	ObjectId() : m_PerTypeId(InvalidId), m_ConstructorId(InvalidId) {}

	PerTypeObjectId m_PerTypeId;
	ConstructorId	m_ConstructorId;
	bool operator<( ObjectId lhs ) const
	{
		if( m_ConstructorId < lhs.m_ConstructorId )
		{
			return true;
		}
		if( m_ConstructorId == lhs.m_ConstructorId )
		{
			return m_PerTypeId < lhs.m_PerTypeId;
		}
		return false;
	}
	bool operator==( const ObjectId& rhs) const
	{
		return (m_ConstructorId == rhs.m_ConstructorId && m_PerTypeId == rhs.m_PerTypeId);
	}
	bool IsValid() const
	{
		return (m_ConstructorId != InvalidId && m_PerTypeId != InvalidId);
	}
	void SetInvalid() 
	{
		m_ConstructorId = InvalidId;
		m_PerTypeId = InvalidId;
	}
};

struct IObjectConstructor
{
	virtual IObject* Construct() = 0;
	virtual void ConstructNull() = 0;	//for use in object replacement, ensures a deleted object can be replaced
	virtual const char* GetName() = 0;
	virtual const char* GetFileName() = 0;
	virtual IObject* GetConstructedObject( PerTypeObjectId num ) const = 0;	//should return 0 for last or deleted object
	virtual size_t	 GetNumberConstructedObjects() const = 0;
	virtual ConstructorId GetConstructorId() const = 0;
	virtual void SetConstructorId( ConstructorId id ) = 0;					//take care how you use this - should only be used by id service
};

struct IPerModuleInterface
{
	virtual const char* GetNameOfModule() const = 0;
	virtual std::vector<IObjectConstructor*>& GetConstructors() = 0;
	virtual void SetSystemTable( SystemTable* pSystemTable ) = 0;

};

#ifdef PLATFORM_MAC
typedef IPerModuleInterface* (*GETPerModuleInterface_PROC)(void);
typedef void*		MODULE_TYPE;
#else
typedef IPerModuleInterface* (__cdecl *GETPerModuleInterface_PROC)(void);
typedef HMODULE		MODULE_TYPE;
#endif


#endif //OBJECTINTERFACE_INCLUDED
