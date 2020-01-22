#define ASSERT_THROW( condition )                             \
{                                                                   \
  if( !( condition ) )                                              \
  {                                                                 \
    throw std::runtime_error(   std::string( __FILE__ )             \
                              + std::string( ":" )                  \
                              + std::to_string( __LINE__ )          \
                              + std::string( " in " )               \
                              + std::string( __PRETTY_FUNCTION__ )  \
    );                                                              \
  }                                                                 \
}

#define ASSERT_STR_STREAM_AND_FILE( x , y )                                   \
{                                                                             \
    std::ifstream expected_out; expected_out.open( y );                       \
    std::string expected_str( (std::istreambuf_iterator<char>(expected_out)), \
    std::istreambuf_iterator<char>());                                        \
    expected_out.close();                                                     \
    if( ( x.str() ) != ( expected_str ) )                                     \
    {                                                                         \
        throw std::runtime_error(   std::string( __FILE__ )                   \
                                  + std::string( ":" )                        \
                                  + std::to_string( __LINE__ )                \
                                  + std::string( " in " )                     \
                                  + std::string( __PRETTY_FUNCTION__ )        \
            );                                                                \
    }                                                                         \
}
