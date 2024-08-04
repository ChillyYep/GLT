#include "RHICommon.h"

ShaderUniformBlockProperty ShaderUniformBlockProperty::s_null;

BlockUniform BlockUniform::s_null;

BlockUniform ShaderUniformBlockProperty::FindBlockUniformByPropertyName(std::string name)
{
	for (int i = 0;i < m_blockUniforms.size();++i)
	{
		if (m_blockUniforms[i].m_uniformName == name)
		{
			return m_blockUniforms[i];
		}
	}
	return BlockUniform::Null();
}

bool ShaderUniformBlockProperty::operator==(const ShaderUniformBlockProperty& other) const
{
	if (other.m_name != m_name)
	{
		return false;
	}

	if (other.m_blockBindingNum != m_blockBindingNum)
	{
		return false;
	}
	for (int i = 0;i < m_blockUniforms.size();++i)
	{
		if (m_blockUniforms[i] != other.m_blockUniforms[i])
		{
			return false;
		}
	}
	return true;
}

bool ShaderUniformBlockProperty::operator!=(const ShaderUniformBlockProperty& other) const
{
	return !(*this == other);
}

bool BlockUniform::operator==(const BlockUniform& other) const
{
	if (other.m_uniformName != m_uniformName || other.m_offset != m_offset || other.m_size != m_size ||
		other.m_type != m_type)
	{
		return false;
	}
	return true;
}

bool BlockUniform::operator!=(const BlockUniform& other) const
{
	return !(*this == other);
}

bool ShaderUniformProperty::operator==(const ShaderUniformProperty& other) const
{
	if (other.m_name != m_name)
	{
		return false;
	}

	if (other.m_location != m_location)
	{
		return false;
	}

	if (other.m_size != m_size)
	{
		return false;
	}
	return true;
}

bool ShaderUniformProperty::operator!=(const ShaderUniformProperty& other) const
{
	return !(*this == other);
}