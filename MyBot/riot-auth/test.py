import sys
import math
import pygame

# Create the window and the two circles
pygame.init()
window = pygame.display.set_mode((800, 600))
circle1 = pygame.Surface((200, 200), pygame.SRCALPHA)
pygame.draw.circle(circle1, (255, 255, 255), (100, 100), 100)
circle2 = pygame.Surface((100, 100), pygame.SRCALPHA)
pygame.draw.circle(circle2, (255, 255, 255), (50, 50), 50)

# Position the circles so that they overlap
circle1_pos = (300, 200)
circle2_pos = (350, 250)

# Set the movement speed for circle1
speed = 0.5
velocity = [0, 0]

# Run the main loop
while True:
    # Handle events
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            sys.exit()

        # Handle keyboard input
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_LEFT:
                velocity[0] = -speed
            elif event.key == pygame.K_RIGHT:
                velocity[0] = speed
            elif event.key == pygame.K_UP:
                velocity[1] = -speed
            elif event.key == pygame.K_DOWN:
                velocity[1] = speed
        elif event.type == pygame.KEYUP:
            if event.key == pygame.K_LEFT or event.key == pygame.K_RIGHT:
                velocity[0] = 0
            elif event.key == pygame.K_UP or event.key == pygame.K_DOWN:
                velocity[1] = 0

    # Update the position of circle1
    circle1_pos = [sum(x) for x in zip(circle1_pos, velocity)]

    # Check for collision
    distance = [x1 - x2 for (x1, x2) in zip(circle1_pos, circle2_pos)]
    intersect_distance = 150
    if math.sqrt(distance[0]**2 + distance[1]**2) <= intersect_distance:
        # The circles are colliding
        circle1.fill((255, 0, 0, 255))
        circle2.fill((255, 0, 0, 255))
    else:
        # The circles are not colliding
        circle1.fill((255, 255, 255, 255))
        circle2.fill((255, 255, 255, 255))

    # Clear the window and draw the circles
    window.fill((0, 0, 0))
    window.blit(circle1, circle1_pos)
    window.blit(circle2, circle2_pos)
    pygame.display.flip()
