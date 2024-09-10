namespace TestProj.Class
{
    /// <summary>
    /// 循环队列
    /// </summary>
    public class CircularFifo<T>
    {
        #region 私有成员
        /// <summary>
        /// buffer
        /// </summary>
        private T[] _buffer;

        /// <summary>
        /// 数据插入索引
        /// </summary>
        private uint _inIdx;

        /// <summary>
        /// 数据弹出索引
        /// </summary>
        private uint _outIdx;

        /// <summary>
        /// Buffer 大小
        /// </summary>
        private uint _size;

        /// <summary>
        /// 队列容量
        /// </summary>
        private uint _capacity;

        #endregion

        #region 公有成员

        /// <summary>
        /// 获取队列容量
        /// </summary>
        public uint Capacity
        {
            get
            {
                return _capacity;
            }
        }

        #endregion

        #region 公有方法

        /// <summary>
        /// 构造方法
        /// </summary>
        /// <param name="capacity">队列容量</param>
        public CircularFifo(uint capacity)
        {
            _buffer = new T[capacity + 1];
            _capacity = capacity;
            _size = capacity + 1;
            _inIdx = 0;
            _outIdx = 0;
        }

        /// <summary>
        /// 获取有效元素数量
        /// </summary>
        /// <returns></returns>
        public uint Count()
        {
            return (_inIdx - _outIdx + _size) % _size;
        }

        /// <summary>
        /// 获取循环队列中剩余空间
        /// </summary>
        /// <returns></returns>
        public uint UnusedCount()
        {
            return (_capacity - Count());
        }

        /// <summary>
        /// 当前队列是否已满
        /// </summary>
        public bool IsFull()
        {
             return ((_inIdx + 1) % _size) == _outIdx;
        }

        /// <summary>
        /// 当前队列是否为空
        /// </summary>
        public bool IsEmpty()
        {
            return (_inIdx == _outIdx);
        }

        /// <summary>
        /// 向循环队列中插入元素
        /// </summary>
        /// <param name="item"></param>
        /// <returns></returns>
        public bool Insert(T item)
        {
            if (IsFull())
            {
                return false;
            }
            _buffer[_inIdx] = item;
            _inIdx = (_inIdx + 1) % _size;
            return true;
        }

        /// <summary>
        /// 从循环队列中移除元素
        /// </summary>
        /// <param name="item"></param>
        /// <returns></returns>
        public bool Remove(ref T item)
        {
            if (IsEmpty())
            {
                return false;
            }
            item = _buffer[_outIdx];
            _outIdx = (_outIdx + 1) % _size;
            return true;
        }

        /// <summary>
        /// 从循环队列中移除元素
        /// </summary>
        /// <returns></returns>
        public bool Remove()
        {
            if (IsEmpty())
            {
                return false;
            }
            _outIdx = (_outIdx + 1) % _size;
            return true;
        }

        /// <summary>
        /// 移除循环队列中的全部元素
        /// </summary>
        /// <returns></returns>
        public T[] RemoveAll()
        {
            T[] ret = new T[Count()];
            {
                for (int i = 0; i < ret.Length; i++)
                {
                    Remove(ref ret[i]);
                }
                return ret;
            }
        }


        /// <summary>
        /// 获取索引位置的元素值，不会从队列中删除元素
        /// </summary>
        /// <param name="item"></param>
        /// <param name="index"></param>
        /// <returns></returns>
        public bool GetValue(ref T item, int index)
        {
            if (index < 0 && index > Count())
            {
                return false;
            }
            // 读取操作, 无需上锁
            item = _buffer[(_outIdx + index) % _size];
            return true;
        }

        /// <summary>
        /// 获取所有元素, 但不会删除元素
        /// </summary>
        /// <returns></returns>
        public T[] GetAll()
        {
            T[] ret = new T[Count()];
            for (int i = 0; i < ret.Length; i++)
            {
                ret[i] = _buffer[(_outIdx + i) % _size];
            }
            return ret;
        }

        /// <summary>
        /// 清除所有元素
        /// </summary>
        public void Clear()
        {
            _inIdx = 0;
            _outIdx = 0;
        }

        #endregion
    }
}

