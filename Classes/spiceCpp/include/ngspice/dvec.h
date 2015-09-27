#ifndef ngspice_DVEC_H
#define ngspice_DVEC_H

#include "ngspice/complex.h"
#include "ngspice/grid.h"


/* Dvec flags. */
enum dvec_flags {
  VF_REAL = (1 << 0),		/* The data is real. */
  VF_COMPLEX = (1 << 1),	/* The data is complex. */
  VF_ACCUM = (1 << 2),		/* writedata should save this vector. */
  VF_PLOT = (1 << 3),		/* writedata should incrementally plot it. */
  VF_PRINT = (1 << 4),		/* writedata should print this vector. */
  VF_MINGIVEN = (1 << 5),	/* The v_minsignal value is valid. */
  VF_MAXGIVEN = (1 << 6),	/* The v_maxsignal value is valid. */
  VF_PERMANENT = (1 << 7)	/* Don't garbage collect this vector. */
};


/* Plot types. */
typedef enum {
    PLOT_LIN, PLOT_COMB, PLOT_POINT
} PLOTTYPE;


/* A (possibly multi-dimensional) data vector.  The data is represented
 * internally by a 1-d array.  The number of dimensions and the size
 * of each dimension is recorded, along with v_length, the total size of
 * the array.  If the dimensionality is 0 or 1, v_length is significant
 * instead of v_numdims and v_dims, and the vector is handled in the old
 * manner.
 */

#define MAXDIMS 8

struct dvec {
    char *v_name;		/* Same as so_vname. */
    int v_type;			/* Same as so_vtype. */
    short v_flags;		/* Flags (a combination of VF_*). */
    double *v_realdata;		/* Real data. */
    ngcomplex_t *v_compdata;	/* Complex data. */
    double v_minsignal;		/* Minimum value to plot. */
    double v_maxsignal;		/* Maximum value to plot. */
    GRIDTYPE v_gridtype;	/* One of GRID_*. */
    PLOTTYPE v_plottype;	/* One of PLOT_*. */
    int v_length;		/* Length of the vector. */
    int v_rlength;		/* How much space we really have. */
    int v_outindex;		/* Index if writedata is building the
				   vector. */
    int v_linestyle;		/* What line style we are using. */
    int v_color;		/* What color we are using. */
    char *v_defcolor;		/* The name of a color to use. */
    int v_numdims;		/* How many dims -- 0 = scalar (len = 1). */
    int v_dims[MAXDIMS];	/* The actual size in each dimension. */
    struct plot *v_plot;	/* The plot structure (if it has one). */
    struct dvec *v_next;	/* Link for list of plot vectors. */
    struct dvec *v_link2;	/* Extra link for things like print. */
    struct dvec *v_scale;	/* If this has a non-standard scale... */
} ;

#define isreal(v)   ((v)->v_flags & VF_REAL)
#define iscomplex(v)    ((v)->v_flags & VF_COMPLEX)

/* list of data vectors being displayed */
struct dveclist {
    struct dvec *vector;
    struct dveclist *next;
};

#endif
