#include <psxtypes/psxtypes.h>
#include <ntapi/ntapi.h>
#include <waio/waio.h>
#include <waio/waio__llapi.h>
#include "waio_impl.h"
#include "waio_cx.h"

#define	WAIO_SLOT_COUNT_TODO	8

struct {
	struct waio_cx_interface	cx;
	struct waio_interface		cx_waio;
	struct waio_slot_interface	cx_slots[WAIO_SLOT_COUNT_TODO];
	struct waio_request_interface	cx_requests[1];
} waio_cx_block;



waio_api waio_cx waio_alloc(
	_in_	void * 		handle,
	_in_	unsigned int	flags,
	_in_	void * 		options,
	_out_	signed int *	status)
{
	int32_t _status;

	/* xvtbl init */
	_status = waio_xvtbls_init_once();

	if (status) *status = _status;
	if (_status) return (waio_cx)0;

	/* stub */
	return (waio_cx)0;
}
