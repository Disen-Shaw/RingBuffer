
/**
 * @file generic_fifo.h
 * @brief General purpose circular queue functionality
 * @author Disen Shaw
 * @version V1.0.2
 * @date 2024-08-13
 */

#include <string.h>

/**
 * @brief Generic Circular FIFO
 */
struct gfifo
{
  void *data;
  unsigned int in;
  unsigned int out;
  unsigned int size;
  unsigned int mask;
};

/**
 * @brief Initialize a generic fifo structure
 *
 * @param[inout] _fifo: fifo's address
 * @param[in] _buf: fifo's buffer
 * @param[in] _size: fifo's size
 */
#define gfifo_init(_fifo, _buf, _size)                                        \
  ({                                                                          \
    typeof (_fifo + 1) _tmp = _fifo;                                          \
    _tmp->data = _buf;                                                        \
    _tmp->in = 0;                                                             \
    _tmp->out = 0;                                                            \
    _tmp->size = _size;                                                       \
    _tmp->mask = _size - 1;                                                   \
  })

/**
 * @brief return true if fifo is empty
 *
 * @param [in] _fifo: fifo's address
 */
#define gfifo_empty(_fifo)                                                    \
  (unsigned int)({                                                            \
    typeof (_fifo + 1) _tmp = _fifo;                                          \
    _tmp->in == _tmp->out;                                                    \
  })

/**
 * @brief return true if fifo is full
 *
 * @param [in] _fifo: fifo's address
 */
#define gfifo_full(_fifo)                                                     \
  (unsigned int)({                                                            \
    typeof (_fifo + 1) _tmp = _fifo;                                          \
    ((_tmp->in + 1) & _tmp->mask) == _tmp->out;                               \
  })

/**
 * @brief return number of element in fifo
 *
 * @param [in] _fifo: fifo's address
 */
#define gfifo_vaild_count(_fifo)                                              \
  (unsigned int)({                                                            \
    typeof (_fifo + 1) _tmp = _fifo;                                          \
    (_tmp->in - _tmp->out) & _tmp->mask;                                      \
  })

/**
 * @brief insert an element into fifo
 *
 * @param [inout] _fifo: fifo's address
 * @param [in] _value: element's address
 * @param [in] _type: element's type
 *
 * @retval:
 *    \li 1: success
 *    \li 0: failed
 */
#define gfifo_insert(_fifo, _value, _type)                                    \
  (unsigned int)({                                                            \
    unsigned int _ret;                                                        \
    typeof (_fifo + 1) _tmp = _fifo;                                          \
    typeof (_value + 1) _tmpv = _value;                                       \
    _ret = !(((_tmp->in + 1) & _tmp->mask) == _tmp->out);                     \
    if (_ret)                                                                 \
      {                                                                       \
        memcpy ((_type *)_tmp->data + _tmp->in, _tmpv, sizeof (_type));       \
        _tmp->in = (_tmp->in + 1) & _tmp->mask;                               \
      }                                                                       \
    _ret;                                                                     \
  })

/**
 * @brief remove an element from fifo
 *
 * @param [inout] _fifo: fifo's address
 * @param [in] _value: element's address
 * @param [in] _type: element's type
 *
 * @retval:
 *    \li 1: success
 *    \li 0: failed
 */
#define gfifo_remove(_fifo, _value, _type)                                    \
  (unsigned int)({                                                            \
    unsigned int _ret;                                                        \
    typeof (_fifo + 1) _tmp = _fifo;                                          \
    typeof (_value + 1) _tmpv = _value;                                       \
    _ret = !(_tmp->in == _tmp->out);                                          \
    if (_ret)                                                                 \
      {                                                                       \
        memcpy (_tmpv, (_type *)_tmp->data + _tmp->out, sizeof (_type));      \
        _tmp->out = (_tmp->out + 1) & _tmp->mask;                             \
      }                                                                       \
    _ret;                                                                     \
  })

/**
 * @brief get an element data without removing
 *
 * @param [inout] _fifo: fifo's address
 * @param [in] _value: element's address
 * @param [in] _type: element's type
 *
 * @retval:
 *    \li 1: success
 *    \li 0: failed
 */
#define gfifo_peek(_fifo, _value, _type)                                      \
  (unsigned int)({                                                            \
    unsigned int _ret;                                                        \
    typeof (_fifo + 1) _tmp = _fifo;                                          \
    typeof (_value + 1) _tmpv = _value;                                       \
    _ret = !(_tmp->in == _tmp->out);                                          \
    if (_ret)                                                                 \
      {                                                                       \
        memcpy (_tmpv, (_type *)_tmp->data + _tmp->out, sizoef (_type));      \
      }                                                                       \
    _ret;                                                                     \
  })

/**
 * @brief remove an element without saving
 *
 * @param [inout] _fifo: fifo's address
 * @param [in] _value: element's address
 * @param [in] _type: element's type
 *
 * @retval:
 *    \li 1: success
 *    \li 0: failed
 */
#define gfifo_throw(_fifo, _type)                                             \
  (unsigned int)({                                                            \
    unsigned int _ret;                                                        \
    typeof (_fifo + 1) _tmp = _fifo;                                          \
    _ret = !(_tmp->in == _tmp->out);                                          \
    if (_ret)                                                                 \
      {                                                                       \
        _tmp->out = (_tmp->out + 1) & _tmp->mask;                             \
      }                                                                       \
    _ret;                                                                     \
  })

/**
 * @brief insert number of element into fifo
 *
 * @param [inout] _fifo: fifo's address
 * @param [in] _value: element's address
 * @param [in] _type: element's type
 *
 * @retval:
 *    \li 1: success
 *    \li 0: failed
 */
#define gfifo_insert_array(_fifo, _array, _len, _type)                        \
  (unsigned int)({                                                            \
    unsigned int _ret;                                                        \
    typeof (_fifo + 1) _tmp = _fifo;                                          \
    typeof (_array + 1) _tmparr = _array;                                     \
    _ret = ((gfifo_vaild_count (_fifo) + _len) < _tmp->size);                 \
    if (_ret)                                                                 \
      {                                                                       \
        if (_tmp->in + _len < _tmp->size)                                     \
          {                                                                   \
            memcpy ((_type *)_tmp->data + _tmp->in, _tmparr,                  \
                    sizeof (_type) * _len);                                   \
          }                                                                   \
        else                                                                  \
          {                                                                   \
            memcpy ((_type *)_tmp->data + _tmp->in, _tmparr,                  \
                    sizeof (_type) * (_tmp->size - _tmp->in));                \
            memcpy ((_type *)_tmp->data,                                      \
                    _tmparr + (_len - (_tmp->size - _tmp->in)),               \
                    sizeof (_type) * (_len - (_tmp->size - _tmp->in)));       \
          }                                                                   \
        _tmp->in = (_tmp->in + _len) & _tmp->mask;                            \
      }                                                                       \
    _ret;                                                                     \
  })

/**
 * @brief remove number of element from fifo
 *
 * @param [inout] _fifo: fifo's address
 * @param [in] _value: element's address
 * @param [in] _type: element's type
 *
 * @retval:
 *    \li 1: success
 *    \li 0: failed
 */
#define gfifo_remove_array(_fifo, _array, _len, _type)                        \
  (unsigned int)({                                                            \
    unsigned int _ret;                                                        \
    typeof (_fifo + 1) _tmp = _fifo;                                          \
    typeof (_array + 1) _tmparr = _array;                                     \
    _ret = (gfifo_vaild_count (_fifo) >= _len);                               \
    {                                                                         \
      if (_ret)                                                               \
        {                                                                     \
          if (_tmp->out + _len < _tmp->size)                                  \
            {                                                                 \
              memcpy (_tmparr, (_type *)_tmp->data + _tmp->out,               \
                      sizeof (_type) * _len);                                 \
            }                                                                 \
          else                                                                \
            {                                                                 \
              memcpy (_tmparr, (_type *)_tmp->data + _tmp->out,               \
                      sizeof (_type) *  (_tmp->size - _tmp->out));    \
              memcpy (_tmparr + (_len - (_tmp->size - _tmp->out)),            \
                      (_type *)_tmp->data,                                    \
                      sizeof (_type) * (_len - (_tmp->size - _tmp->out)));    \
            }                                                                 \
          _tmp->out = (_tmp->out + _len) & _tmp->mask;                        \
        }                                                                     \
    }                                                                         \
    _ret;                                                                     \
  })
