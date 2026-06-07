/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/21 16:33:54 by baelgadi          #+#    #+#             */
/*   Updated: 2026/04/12 19:01:49 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KEYS_BONUS_H
# define KEYS_BONUS_H

# define KEY_W		119
# define KEY_A		97
# define KEY_S		115
# define KEY_D		100
# define KEY_LEFT	65361
# define KEY_RIGHT	65363
# define KEY_ESC	65307

/* SECTION 4 (include/keys_bonus.h): extra keys for bonus features.
 *   KEY_E - interact / toggle the door the player faces.
 *   KEY_M - toggle the minimap on and off. */
# define KEY_E		101
# define KEY_M		109
/* SECTION 4 (include/keys_bonus.h): KEY_SPACE fires the current weapon
 * (drains the charge bar while held). Left-click fires too (button hook). */
# define KEY_SPACE	32
/* SECTION 4 (include/keys_bonus.h): KEY_ENTER (X11 Return) confirms the
 * dead/win screen — respawn the floor or advance to the next one. */
# define KEY_ENTER	65293
/* SECTION 4 (include/keys_bonus.h): front-end menu navigation. Up/Down move the
 * highlight (W/S work too), KEY_BACKSPACE steps back to the main menu. */
# define KEY_UP			65362
# define KEY_DOWN		65364
# define KEY_BACKSPACE	65288

#endif
