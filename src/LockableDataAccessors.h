#pragma once

#include <mutex>
#include <shared_mutex>
#include <type_traits>

enum class EAccessMode
{
	Read,
	Write
};

template<typename DataType, EAccessMode AccessMode>
class DataAccessor
{
public:
	DataAccessor() = delete;
	DataAccessor(const DataAccessor& other) = delete;
	DataAccessor& operator=(const DataAccessor& other) = delete;

	DataType* operator->() const noexcept { return m_Data; }

protected:
	DataAccessor(DataType& data, std::shared_mutex& mutex) : m_Data(&data), m_Lock(mutex)
	{
	}

	DataAccessor(DataAccessor&& other) noexcept : m_Data(other.m_Data), m_Lock(std::move(other.m_Lock))
	{
		other.m_Data = nullptr;
	}

	virtual ~DataAccessor() = default;

	DataAccessor& operator=(DataAccessor&& other) noexcept
	{
		m_Data = other.m_Data;
		other.m_Data = nullptr;

		m_Lock = std::move(other.m_Lock);

		return *this;
	}

private:
	DataType* m_Data;

	using LockType = std::conditional_t<AccessMode == EAccessMode::Read, std::shared_lock<std::shared_mutex>, std::unique_lock<std::shared_mutex>>;
	LockType m_Lock;
};

template<typename DataType>
class DataReader final : public DataAccessor<const DataType, EAccessMode::Read>
{
	using Super = DataAccessor<const DataType, EAccessMode::Read>;

public:
	DataReader() = delete;
	DataReader(const DataReader& other) = delete;

	DataReader(const DataType& data, std::shared_mutex& mutex): Super(data, mutex)
	{

	}

	DataReader(DataReader&& other) noexcept : Super(std::move(other))
	{

	}

	DataReader& operator=(const DataReader& other) = delete;
	DataReader& operator=(DataReader&& other) noexcept
	{
		Super::operator=(std::move(other));
		return *this;
	}

	~DataReader() override = default;
};

template<typename DataType>
class DataWriter final : public DataAccessor<DataType, EAccessMode::Write>
{
	using Super = DataAccessor<DataType, EAccessMode::Write>;

public:
	DataWriter() = delete;
	DataWriter(const DataWriter& other) = delete;

	DataWriter(DataType& data, std::shared_mutex& mutex) : Super(data, mutex)
	{

	}

	DataWriter(DataWriter&& other) noexcept : Super(std::move(other))
	{

	}

	DataWriter& operator=(const DataWriter& other) = delete;
	DataWriter& operator=(DataWriter&& other) noexcept
	{
		Super::operator=(std::move(other));
		return *this;
	}

	~DataWriter() override = default;
};