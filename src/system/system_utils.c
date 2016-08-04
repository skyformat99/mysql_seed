#include "system/system_utils.h"

/* socket */
extern inline bool
socket_status(int *const restrict socket_descriptor,
	      const int domain,
	      const int type,
	      const int protocol);
extern inline void
socket_muffle(int *const restrict socket_descriptor,
	      const int domain,
	      const int type,
	      const int protocol);
extern inline bool
socket_report(int *const restrict socket_descriptor,
	      const int domain,
	      const int type,
	      const int protocol,
	      const char *restrict *const restrict failure);
extern inline void
socket_handle(int *const restrict socket_descriptor,
	      const int domain,
	      const int type,
	      const int protocol,
	      Handler *const handle,
	      void *arg);
extern inline void
socket_handle_cl(int *const restrict socket_descriptor,
		 const int domain,
		 const int type,
		 const int protocol,
		 const struct HandlerClosure *const restrict fail_cl);

#ifndef WIN32
/* get_interface_addresses */
extern inline bool
get_interface_addresses_status(struct ifconf *const restrict configuration,
			       const int device_descriptor);
extern inline void
get_interface_addresses_muffle(struct ifconf *const restrict configuration,
			       const int device_descriptor);
extern inline bool
get_interface_addresses_report(struct ifconf *const restrict configuration,
			       const int device_descriptor,
			       const char *restrict *const restrict failure);
extern inline void
get_interface_addresses_handle(struct ifconf *const restrict configuration,
			       const int device_descriptor,
			       Handler *const handle,
			       void *arg);
extern inline void
get_interface_addresses_handle_cl(struct ifconf *const restrict configuration,
				  const int device_descriptor,
				  const struct HandlerClosure *const restrict fail_cl);

/* get_device_active_flags */
extern inline bool
get_device_active_flags_status(struct ifreq *const restrict request,
			       const int device_descriptor);
extern inline void
get_device_active_flags_muffle(struct ifreq *const restrict request,
			       const int device_descriptor);
extern inline bool
get_device_active_flags_report(struct ifreq *const restrict request,
			       const int device_descriptor,
			       const char *restrict *const restrict failure);
extern inline void
get_device_active_flags_handle(struct ifreq *const restrict request,
			       const int device_descriptor,
			       Handler *const handle,
			       void *arg);
extern inline void
get_device_active_flags_handle_cl(struct ifreq *const restrict request,
				  const int device_descriptor,
				  const struct HandlerClosure *const restrict fail_cl);

/* get_winsize */
extern inline bool
get_winsize_status(struct winsize *const restrict window,
		   const int file_descriptor);
extern inline void
get_winsize_muffle(struct winsize *const restrict window,
		   const int file_descriptor);
extern inline bool
get_winsize_report(struct winsize *const restrict window,
		   const int file_descriptor,
		   const char *restrict *const restrict failure);
extern inline void
get_winsize_handle(struct winsize *const restrict window,
		   const int file_descriptor,
		   Handler *const handle,
		   void *arg);
extern inline void
get_winsize_handle_cl(struct winsize *const restrict window,
		      const int file_descriptor,
		      const struct HandlerClosure *const restrict fail_cl);
#endif /* ifndef WIN32 */

#ifdef LINUX
/* get_hardware_address */
extern inline bool
get_hardware_address_status(struct ifreq *const restrict request,
			    const int device_descriptor);
extern inline void
get_hardware_address_muffle(struct ifreq *const restrict request,
			    const int device_descriptor);
extern inline bool
get_hardware_address_report(struct ifreq *const restrict request,
			    const int device_descriptor,
			    const char *restrict *const restrict failure);
extern inline void
get_hardware_address_handle(struct ifreq *const restrict request,
			    const int device_descriptor,
			    Handler *const handle,
			    void *arg);
extern inline void
get_hardware_address_handle_cl(struct ifreq *const restrict request,
			       const int device_descriptor,
			       const struct HandlerClosure *const restrict fail_cl);
#endif /* ifdef LINUX */


/* getaddrinfo */
extern inline bool
getaddrinfo_status(const char *const node,
		   const char *const service,
		   const struct addrinfo *const hints,
		   struct addrinfo **const result);
extern inline void
getaddrinfo_muffle(const char *const node,
		   const char *const service,
		   const struct addrinfo *const hints,
		   struct addrinfo **const result);
extern inline bool
getaddrinfo_report(const char *const node,
		   const char *const service,
		   const struct addrinfo *const hints,
		   struct addrinfo **const result,
		   const char *restrict *const restrict failure);
extern inline void
getaddrinfo_handle(const char *const node,
		   const char *const service,
		   const struct addrinfo *const hints,
		   struct addrinfo **const result,
		   Handler *const handle,
		   void *arg);
extern inline void
getaddrinfo_handle_cl(const char *const node,
		      const char *const service,
		      const struct addrinfo *const hints,
		      struct addrinfo **const result,
		      const struct HandlerClosure *const restrict fail_cl);
