#pragma once

enum class TAG_REQUEST
{
    TEST_CONNECTION,
    RECEIVE_MY_ID,

    SEND_ID,
    SEND_NICKNAME,
    SEND_POSITION_AND_ROTATION,
    SEND_HEAD_ANGLE,
    SEND_UPPER_ANIMATION_INDEX,
    SEND_LOWER_ANIMATION_INDEX,
    SEND_IS_DEAD,

    SEND_EVENT_FIRE_BULLET,
    SEND_EVENT_SOUND,
    SEND_EVENT_MINUS_DAMAGE

};


