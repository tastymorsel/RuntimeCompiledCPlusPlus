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

#ifndef OBJECTINTERFACEPERMODULE_INCLUDED
#define OBJECTINTERFACEPERMODULE_INCLUDED

#include "ObjectInterface.h"
#include <string>
#include <vector>
#include <assert.h>

#define AU_ASSERT( statement )  do { if (!(statement)) { int a = *((int*)(0)); } } while(0) 


class PerModuleInterface : public IPerModuleInterface
{
public:
	static PerModuleInterface* GetInstance();

	void AddConstructor( IObjectConstructor* pConstructor );

	void SetNameOfModule( const char* name_ );

	virtual const char* GetNameOfModule() const;

	virtual std::vector<IObjectConstructor*>& GetConstructors();
	virtual void SetSystemTable( SystemTable* pSystemTable );

	SystemTable* GetSystemTable()
	{
		return m_pSystemTable;
	}



private:
	PerModuleInterface();

	~PerModuleInterface()
	{
	}


	static PerModuleInterface*		ms_pObjectManager;
	std::vector<IObjectConstructor*>	m_ObjectConstructors;
	std::string						m_Name;
	SystemTable*					m_pSystemTable;
};


template<typename T> class TObjectConstructorConcrete: public IObjectConstructor
{
public:
	TObjectConstructorConcrete( const char* Filename ) : m_FileName( Filename )
	{
		PerModuleInterface::GetInstance()->AddConstructor( this );
		m_Id = InvalidId;
	}

	virtual IObject* Construct()
	{
		T* pT = 0;
		if( m_FreeIds.empty() )
		{
			PerTypeObjectId id = m_ConstructedObjects.size();
			pT = new T();
			pT->SetPerTypeId( id );
			m_ConstructedObjects.push_back( pT );
		}
		else
		{
			PerTypeObjectId id = m_FreeIds.back();
			m_FreeIds.pop_back();
			pT = new T();
			pT->SetPerTypeId( id );
			AU_ASSERT( 0 == m_ConstructedObjects[ id ] );
			m_ConstructedObjects[ id ] = pT;

		}
		return pT;
	}

	virtual void ConstructNull()
	{
		m_ConstructedObjects.push_back( NULL );
	}

	virtual const char* GetName()
	{
		return T::GetTypeNameStatic();
	}

	virtual const char* GetFileName()
	{
		return m_FileName.c_str();
	}
	virtual IObject* GetConstructedObject( PerTypeObjectId id ) const
	{
		if( m_ConstructedObjects.size() > id )
		{
			return m_ConstructedObjects[id];
		}
		return 0;
	}
	virtual size_t	 GetNumberConstructedObjects() const
	{
		return m_ConstructedObjects.size();
	}
	virtual ConstructorId GetConstructorId() const
	{
		return m_Id;
	}
	virtual void SetConstructorId( ConstructorId id )
	{
		if( InvalidId == m_Id )
		{
			m_Id = id;
		}
	}


	void DeRegister( PerTypeObjectId id )
	{
		//remove from constructed objects.
		//use swap with last one
		if( m_ConstructedObjects.size() - 1 == id )
		{
			//it's the last one, just remove it.
			m_ConstructedObjects.pop_back();
		}
		else
		{
			m_FreeIds.push_back( id );
			m_ConstructedObjects[ id ] = 0;
		}
	}
private:
	std::string				m_FileName;
	std::vector<T*>			m_ConstructedObjects;
	std::vector<PerTypeObjectId>	m_FreeIds;
	ConstructorId			m_Id;
};


template<typename T> class TActual : public T
{
public:
	virtual ~TActual() { m_Constructor.DeRegister( m_Id ); }
	virtual PerTypeObjectId GetPerTypeId() const { return m_Id; }
	virtual IObjectConstructor* GetConstructor() const { return &m_Constructor; }
	static const char* GetTypeNameStatic();
	virtual const char* GetTypeName() const
	{
		return GetTypeNameStatic();
	}
	void SetPerTypeId( PerTypeObjectId id ) { m_Id = id; }
private:
	PerTypeObjectId m_Id;
	static TObjectConstructorConcrete< TActual<T> > m_Constructor;
};

//NOTE: the file macro will only emit the full path if /FC option is used in visual studio or /ZI (Which forces /FC)
#define REGISTERCLASS(T)	template class TActual< T >; \
		template<> TObjectConstructorConcrete< TActual<T> > TActual<T>::m_Constructor( __FILE__ ); \
		template<> const char* TActual<T>::GetTypeNameStatic() { return #T; }


#endif OBJECTINTERFACEPERMODULE_INCLUDED
