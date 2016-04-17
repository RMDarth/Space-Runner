set( filename "build-number.txt" )

# check for the previous build number
if ( EXISTS "${filename}" )
	# read build number from file
	file( READ "${filename}" build_number LIMIT 10 )
else()
	# no file with build number, assuming very first build
	set( build_number 0 )
endif()

# increment build number
math( EXPR build_number "${build_number} + 1" )

# save new build number into the same file
file( WRITE "${filename}" "${build_number}" )