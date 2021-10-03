void
usage ( int status )
{
if ( status != EXIT_SUCCESS )
emit_try_help () ;
else
{
printf ( _ ( lr_1 ) , program_name ) ;
fputs ( _ ( lr_2 ) , stdout ) ;
fputs ( _ ( lr_3 ) , stdout ) ;
fputs ( HELP_OPTION_DESCRIPTION , stdout ) ;
fputs ( VERSION_OPTION_DESCRIPTION , stdout ) ;
printf ( USAGE_BUILTIN_WARNING , PROGRAM_NAME ) ;
emit_ancillary_info () ;
}
exit ( status ) ;
}
static void
file_name_free ( struct file_name * p )
{
free ( p -> buf ) ;
free ( p ) ;
}
static struct file_name *
file_name_init ( void )
{
struct file_name * p = xmalloc ( sizeof *p ) ;
p -> n_alloc = MIN ( 2 * PATH_MAX , 32 * 1024 ) ;
p -> buf = xmalloc ( p -> n_alloc ) ;
p -> start = p -> buf + ( p -> n_alloc - 1 ) ;
p -> start [ 0 ] = '\0' ;
return p ;
}
static void
file_name_prepend ( struct file_name * p , char const * s , size_t s_len )
{
size_t n_free = p -> start - p -> buf ;
if ( n_free < 1 + s_len )
{
size_t half = p -> n_alloc + 1 + s_len ;
char * q = xnmalloc ( 2 , half ) ;
size_t n_used = p -> n_alloc - n_free ;
p -> start = q + 2 * half - n_used ;
memcpy ( p -> start , p -> buf + n_free , n_used ) ;
free ( p -> buf ) ;
p -> buf = q ;
p -> n_alloc = 2 * half ;
}
p -> start -= 1 + s_len ;
p -> start [ 0 ] = '/' ;
memcpy ( p -> start + 1 , s , s_len ) ;
}
static char *
nth_parent ( size_t n )
{
char * buf = xnmalloc ( 3 , n ) ;
char * p = buf ;
size_t i ;
for ( i = 0 ; i < n ; i ++ )
{
memcpy ( p , lr_4 , 3 ) ;
p += 3 ;
}
p [ - 1 ] = '\0' ;
return buf ;
}
static void
find_dir_entry ( struct stat * dot_sb , struct file_name * file_name ,
size_t parent_height )
{
DIR * dirp ;
int fd ;
struct stat parent_sb ;
bool use_lstat ;
bool found ;
dirp = opendir ( lr_5 ) ;
if ( dirp == NULL )
error ( EXIT_FAILURE , errno , _ ( lr_6 ) ,
quote ( nth_parent ( parent_height ) ) ) ;
fd = dirfd ( dirp ) ;
if ( ( 0 <= fd ? fchdir ( fd ) : chdir ( lr_5 ) ) < 0 )
error ( EXIT_FAILURE , errno , _ ( lr_7 ) ,
quote ( nth_parent ( parent_height ) ) ) ;
if ( ( 0 <= fd ? fstat ( fd , & parent_sb ) : stat ( lr_8 , & parent_sb ) ) < 0 )
error ( EXIT_FAILURE , errno , _ ( lr_9 ) ,
quote ( nth_parent ( parent_height ) ) ) ;
use_lstat = ( parent_sb . st_dev != dot_sb -> st_dev ) ;
found = false ;
while ( 1 )
{
struct dirent const * dp ;
struct stat ent_sb ;
ino_t ino ;
errno = 0 ;
if ( ( dp = readdir_ignoring_dot_and_dotdot ( dirp ) ) == NULL )
{
if ( errno )
{
int e = errno ;
closedir ( dirp ) ;
errno = e ;
dirp = NULL ;
}
break;
}
ino = D_INO ( dp ) ;
if ( ino == NOT_AN_INODE_NUMBER || use_lstat )
{
if ( lstat ( dp -> d_name , & ent_sb ) < 0 )
{
continue;
}
ino = ent_sb . st_ino ;
}
if ( ino != dot_sb -> st_ino )
continue;
if ( ! use_lstat || ent_sb . st_dev == dot_sb -> st_dev )
{
file_name_prepend ( file_name , dp -> d_name , _D_EXACT_NAMLEN ( dp ) ) ;
found = true ;
break;
}
}
if ( dirp == NULL || closedir ( dirp ) != 0 )
{
error ( EXIT_FAILURE , errno , _ ( lr_10 ) ,
quote ( nth_parent ( parent_height ) ) ) ;
}
if ( ! found )
error ( EXIT_FAILURE , 0 ,
_ ( lr_11 ) ,
quote ( nth_parent ( parent_height ) ) ) ;
* dot_sb = parent_sb ;
}
static void
robust_getcwd ( struct file_name * file_name )
{
size_t height = 1 ;
struct dev_ino dev_ino_buf ;
struct dev_ino * root_dev_ino = get_root_dev_ino ( & dev_ino_buf ) ;
struct stat dot_sb ;
if ( root_dev_ino == NULL )
error ( EXIT_FAILURE , errno , _ ( lr_12 ) ,
quote ( lr_13 ) ) ;
if ( stat ( lr_8 , & dot_sb ) < 0 )
error ( EXIT_FAILURE , errno , _ ( lr_9 ) , quote ( lr_8 ) ) ;
while ( 1 )
{
if ( SAME_INODE ( dot_sb , * root_dev_ino ) )
break;
find_dir_entry ( & dot_sb , file_name , height ++ ) ;
}
if ( file_name -> start [ 0 ] == '\0' )
file_name_prepend ( file_name , lr_14 , 0 ) ;
}
static char *
logical_getcwd ( void )
{
struct stat st1 ;
struct stat st2 ;
char * wd = getenv ( lr_15 ) ;
char * p ;
if ( ! wd || wd [ 0 ] != '/' )
return NULL ;
p = wd ;
while ( ( p = strstr ( p , lr_16 ) ) )
{
if ( ! p [ 2 ] || p [ 2 ] == '/'
|| ( p [ 2 ] == '.' && ( ! p [ 3 ] || p [ 3 ] == '/' ) ) )
return NULL ;
p ++ ;
}
if ( stat ( wd , & st1 ) == 0 && stat ( lr_8 , & st2 ) == 0 && SAME_INODE ( st1 , st2 ) )
return wd ;
return NULL ;
}
int
main ( int argc , char * * argv )
{
char * wd ;
bool logical = ( getenv ( lr_17 ) != NULL ) ;
initialize_main ( & argc , & argv ) ;
set_program_name ( argv [ 0 ] ) ;
setlocale ( LC_ALL , lr_14 ) ;
bindtextdomain ( PACKAGE , LOCALEDIR ) ;
textdomain ( PACKAGE ) ;
atexit ( close_stdout ) ;
while ( 1 )
{
int c = getopt_long ( argc , argv , lr_18 , longopts , NULL ) ;
if ( c == - 1 ) MST[EQ$@1$@2$!LT$@1$@2$]MSP[N]
break;
switch ( c )
{
case 'L' :
logical = true ;
break;
case 'P' :
logical = false ;
break;
case_GETOPT_HELP_CHAR ;
case_GETOPT_VERSION_CHAR ( PROGRAM_NAME , AUTHORS ) ;
default:
usage ( EXIT_FAILURE ) ;
}
}
if ( optind < argc )
error ( 0 , 0 , _ ( lr_19 ) ) ;
if ( logical )
{
wd = logical_getcwd () ;
if ( wd )
{
puts ( wd ) ;
exit ( EXIT_SUCCESS ) ;
}
}
wd = xgetcwd () ;
if ( wd != NULL )
{
puts ( wd ) ;
free ( wd ) ;
}
else
{
struct file_name * file_name = file_name_init () ;
robust_getcwd ( file_name ) ;
puts ( file_name -> start ) ;
file_name_free ( file_name ) ;
}
exit ( EXIT_SUCCESS ) ;
}
