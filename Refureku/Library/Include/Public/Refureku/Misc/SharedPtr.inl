/**
*	Copyright (c) 2021 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the Refureku library project which is released under the MIT License.
*	See the README.md file for full license details.
*/

template<typename T, typename... Args>
SharedPtr<T> rfk::makeShared(Args&&... args)
{
	return std::make_shared<T>(std::forward<Args>(args)...);
}