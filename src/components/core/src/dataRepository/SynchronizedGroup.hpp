/**
 * @file DataObjectManager.h
 * @date created on Nov 21, 2014
 * @author Randolph R. Settgast
 */


#ifndef DATAOBJECTMANAGER_H_
#define DATAOBJECTMANAGER_H_

#include <iostream>

#include "ObjectCatalog.hpp"
#include "ViewWrapper.hpp"

//#include "CodingUtilities/ANSTexception.hpp"

#ifndef USE_DYNAMIC_CASTING
#define USE_DYNAMIC_CASTING 1;
#endif

#ifndef NOCHARTOSTRING_KEYLOOKUP
#define NOCHARTOSTRING_KEYLOOKUP 1
#endif

/**
 * namespace to encapsulate functions in simulation tools
 */
namespace geosx
{
namespace dataRepository
{

/**
 * @author Randolph R. Settgast
 *
 * class that encapsulates and manages a collection of DataObjects. Can be considered a "node" in a
 * hierarchy of managers that represent physical groupings of data.
 *
 */
class SynchronizedGroup
{
public:
  /**
   * @name constructors, destructor, copy, move, assignments
   */
  ///@{

  /**
   * @author Randolph R. Settgast
   * @param name the name of this object manager
   */
  explicit SynchronizedGroup( std::string const & name,
                              SynchronizedGroup * const parent );

  /**
   *
   */
  virtual ~SynchronizedGroup();

  /**
   *
   * @param source source WrapperCollection
   */
  SynchronizedGroup( SynchronizedGroup&& source );


  SynchronizedGroup() = delete;
  SynchronizedGroup( SynchronizedGroup const & source ) = delete;
  SynchronizedGroup& operator=( SynchronizedGroup const & ) = delete;
  SynchronizedGroup& operator=(SynchronizedGroup&&) = delete;

  ///@}


  using CatalogInterface = cxx_utilities::CatalogInterface< SynchronizedGroup, std::string const &, SynchronizedGroup * const >;
  static CatalogInterface::CatalogType& GetCatalog();


  virtual void Registration( dataRepository::SynchronizedGroup * const )
  {}

  virtual const std::type_info& get_typeid() const
  {
    return typeid(*this);
  }


  template< typename T = SynchronizedGroup >
  T& RegisterGroup( std::string const & name, std::unique_ptr<T> newObject );

  template< typename T = SynchronizedGroup >
  T& RegisterGroup( std::string const & name )
  {
    return RegisterGroup<T>( name, std::move(std::make_unique< T >( name, this )) );
  }

  template< typename T = SynchronizedGroup >
  T& RegisterGroup( std::string const & name, std::string const & catalogName )
  {
    std::unique_ptr<T> newGroup = T::CatalogInterface::Factory(catalogName, name, this );
    return RegisterGroup<T>( name, std::move(newGroup) );
  }



  template< typename T = SynchronizedGroup >
  T& GetGroup( std::string const & name )
  {
#ifdef USE_DYNAMIC_CASTING
    return dynamic_cast<T&>( *(m_subObjectManagers.at(name)) );
#else
    return static_cast<T&>( *(m_subObjectManagers.at(name)) );
#endif
  }



  template< typename T = SynchronizedGroup >
  T const & GetGroup( std::string const & name ) const
  {
#ifdef USE_DYNAMIC_CASTING
    return dynamic_cast<T const &>( *(m_subObjectManagers.at(name)) );
#else
    return static_cast<T const &>( *(m_subObjectManagers.at(name)) );
#endif
  }


  template< typename T >
  ViewWrapper<T>& RegisterViewWrapper( std::string const & name, std::size_t * const rkey = nullptr );


  ViewWrapperBase& RegisterViewWrapper( std::string const & name, rtTypes::TypeIDs const & type );


  //***********************************************************************************************

  template< typename T >
  ViewWrapper<T> const & getWrapper( std::size_t const index ) const
  {
#ifdef USE_DYNAMIC_CASTING
    return dynamic_cast< ViewWrapper<T> const & >( *(m_wrappers[index]) );
#else
    return static_cast< ViewWrapper<T> const & >( *(m_wrappers[index]) );
#endif
  }

  template< typename T >
  ViewWrapper<T> & getWrapper( std::size_t const index )
  {
    return const_cast<ViewWrapper<T>&>( const_cast< SynchronizedGroup const *>(this)->getWrapper<T>( index ) );
  }

  template< typename T >
  ViewWrapper<T> const & getWrapper( std::string const & name ) const
  {
    auto index = m_keyLookup.at(name);
    return getWrapper<T>(index);
  }

  template< typename T >
  ViewWrapper<T>& getWrapper( std::string const & name )
  { return const_cast<ViewWrapper<T>&>( const_cast<const SynchronizedGroup*>(this)->getWrapper<T>( name ) ); }



  template< typename T >
  typename ViewWrapper<T>::rtype_const getData( std::size_t const index ) const
  {
    return getWrapper<T>(index).data();
  }

  template< typename T >
  typename ViewWrapper<T>::rtype getData( std::size_t const index )
  {
    return getWrapper<T>(index).data();
//    return const_cast<typename WrapperView<T>::rtype>( const_cast<const SynchronizedGroup*>(this)->getData<T>( index ) );
  }

  template< typename T >
  typename ViewWrapper<T>::rtype_const getData( std::string const & name ) const
  {
    auto index = m_keyLookup.at(name);
    return getData<T>( index );
  }

  template< typename T >
  typename ViewWrapper<T>::rtype getData( std::string const & name )
  {
    auto index = m_keyLookup.at(name);
    return getData<T>( index );
  }

  template< typename T >
  T const & getReference( std::size_t const index ) const
  {
    return getWrapper<T>(index).reference();
  }

  template< typename T >
  T& getReference( std::size_t const index )
  {
    return const_cast<T&>( const_cast<const SynchronizedGroup*>(this)->getReference<T>( index ) );
  }

  template< typename T >
  T const & getReference( std::string const & name ) const
  {
    auto index = m_keyLookup.at(name);
    return getReference<T>( index );
  }

  template< typename T >
  T & getReference( std::string const & name )
  {
    auto index = m_keyLookup.at(name);
    return getReference<T>( index );
  }

  void resize( std::size_t newsize );

  inline std::size_t size() const
  {
    return *(getData<std_size_t>(keys::size));
  }


//#include "Common/Common.h"
//  template< FieldKey FIELDKEY>
//  typename Wrapper<TYPE>::rtype GetFieldData( )
//  {
//    return GetFieldData< array<typename Field<FIELDKEY>::Type> >(Field<FIELDKEY>::Name());
//  }



  asctoolkit::sidre::DataGroup * getSidreGroup()              { return m_sidreGroup; }
  asctoolkit::sidre::DataGroup const * getSidreGroup() const  { return m_sidreGroup; }

  asctoolkit::sidre::DataGroup * setSidreGroup()
  {
    return m_sidreGroup;
  }

  SynchronizedGroup * getParent()             { return m_parent; }
  SynchronizedGroup const * getParent() const { return m_parent; }

  SynchronizedGroup * setParent( SynchronizedGroup * const parent )
  {
    m_parent = parent;
    m_sidreGroup = m_parent->getSidreGroup();

    return m_parent;
  }

private:
  std::unordered_map<std::string,std::size_t> m_keyLookup;
  std::vector< std::unique_ptr<ViewWrapperBase> > m_wrappers;

  SynchronizedGroup* m_parent = nullptr;
  std::unordered_map< std::string, std::unique_ptr<SynchronizedGroup> > m_subObjectManagers;

  asctoolkit::sidre::DataGroup* m_sidreGroup;

#if NOCHARTOSTRING_KEYLOOKUP == 1

  template< typename T = SynchronizedGroup >
  T const & GetGroup( char const * ) const;

  template< typename T = SynchronizedGroup >
  T& GetGroup( char const * name );


  template< typename T >
  typename ViewWrapper<T>::rtype_const getData( char const * ) const;

  template< typename T >
  typename ViewWrapper<T>::rtype getData( char const * );

  template< typename T >
  T const & getReference( char const * ) const;

  template< typename T >
  T & getReference( char const * );


  template< typename T >
  ViewWrapper<T> const & getWrapper( char const * ) const;
  template< typename T >

  ViewWrapper<T>& getWrapper( char const * );


#endif
};



template< typename T >
ViewWrapper<T>& SynchronizedGroup::RegisterViewWrapper( std::string const & name, std::size_t * const rkey )
{
  std::size_t key = static_cast<std::size_t>(-1);

  auto iterKeyLookup = m_keyLookup.find(name);

  // if the key was not found, make DataObject<T> and insert
  if( iterKeyLookup == m_keyLookup.end() )
  {
    m_wrappers.push_back( std::move( ViewWrapper<T>::Factory(name,this) ) );
    key = m_wrappers.size() - 1;
    m_keyLookup.insert( std::make_pair(name,key) );
    m_wrappers.back()->resize(this->size());
  }
  // if key was found, make sure that they are the same type
  else
  {
    key = m_keyLookup.at(name);
    auto& basePtr = m_wrappers[key];
    if( typeid(T) != basePtr->get_typeid() )
    {
      std::cout<<LOCATION<<std::endl;
      throw std::exception();
    }
  }

  if( rkey != nullptr )
  {
    *rkey = key;
  }
  return getWrapper<T>(key);
}

template< typename T >
T& SynchronizedGroup::RegisterGroup( std::string const & name,
                                     std::unique_ptr<T> newObject )
{
  auto iterKeyLookup = m_subObjectManagers.find(name);

  // if the key was not found, make DataObject<T> and insert
  if( iterKeyLookup == m_subObjectManagers.end() )
  {
    auto insertResult = m_subObjectManagers.insert( std::make_pair( name, std::move(newObject) ) );

    if( !insertResult.second )
    {
      std::cout<<LOCATION<<std::endl;
      throw std::exception();
    }
    iterKeyLookup = insertResult.first;
//    iterKeyLookup->second.get()->
  }
  // if key was found, make sure that they are the same type
  else
  {

    if( typeid(T) != iterKeyLookup->second->get_typeid() )
    {
      std::cout<<LOCATION<<std::endl;
      throw std::exception();
    }
  }
#ifdef USE_DYNAMIC_CASTING
  return *(dynamic_cast<T*>( (iterKeyLookup->second).get() ) );
#else
  return *(static_cast<T*>( (iterKeyLookup->second).get() ) );
#endif
}

} // namespace dataRepository
} /* namespace geosx */

#endif /* DATAOBJECTMANAGER_H_ */