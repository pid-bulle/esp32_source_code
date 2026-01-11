#!/usr/bin/env python3
"""
Sterowanie robotem za pomocą klawiatury WASD
Wymaga: pip install pygame requests
"""
import requests
import pygame
import sys

# Konfiguracja
IP = "10.20.7.157"


def cmd(endpoint, value=None):
    """Wysyła komendę do robota"""
    try:
        if value is not None:
            url = f"http://{IP}/{endpoint}/{value}"
        else:
            url = f"http://{IP}/{endpoint}"
        requests.get(url, timeout=0.1)
    except:
        pass


def main():
    pygame.init()
    screen = pygame.display.set_mode((400, 300))
    pygame.display.set_caption("Robot Control - WASD")
    clock = pygame.time.Clock()

    print("=== STEROWANIE ROBOTEM ===")
    print(f"Robot IP: {IP}")
    print("\nOkno pygame otwarte - kliknij w nie i użyj klawiszy:")
    print("  W       - jazda do przodu")
    print("  A       - obrót w lewo")
    print("  D       - obrót w prawo")
    print("  W+A     - jazda ze skrętem w lewo")
    print("  W+D     - jazda ze skrętem w prawo")
    print("  Q / ESC - wyjście")
    print("\nPuszczenie = STOP\n")

    font = pygame.font.Font(None, 36)

    # Zapamiętaj poprzedni stan
    prev_status = None

    running = True
    while running:
        # Obsługa zdarzeń
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False
            elif event.type == pygame.KEYDOWN:
                if event.key == pygame.K_ESCAPE or event.key == pygame.K_q:
                    running = False

        # Sprawdź wciśnięte klawisze
        keys = pygame.key.get_pressed()
        w = keys[pygame.K_w]
        a = keys[pygame.K_a]
        d = keys[pygame.K_d]

        # Logika ruchu
        status = ""
        if w and not a and not d:
            status = "W"
        elif a and not w and not d:
            status = "A"
        elif d and not w and not a:
            status = "D"
        elif w and a and not d:
            status = "WA"
        elif w and d and not a:
            status = "WD"
        else:
            status = "STOP"

        # Wyślij komendę tylko gdy stan się zmienił
        if status != prev_status:
            if status == "W":
                cmd("speed", 128)
                cmd("turn", 0)

                display = "↑ PRZÓD"
            elif status == "A":
                cmd("speed", 0)
                cmd("turn", 0)
                display = "← LEWO"
            elif status == "D":
                cmd("speed", 0)
                cmd("turn", 255)
                display = "→ PRAWO"
            elif status == "WA":
                cmd("speed", 200)
                cmd("turn", 0)
                display = "↑← PRZÓD-LEWO"
            elif status == "WD":
                cmd("speed", 200)
                cmd("turn", 255)
                display = "↑→ PRZÓD-PRAWO"
            else:
                cmd("speed", 0)
                cmd("turn", 128)
                display = "■ STOP"

            prev_status = status
        else:
            # Użyj poprzedniego wyświetlania
            if status == "W":
                display = "↑ PRZÓD"
            elif status == "A":
                display = "← LEWO"
            elif status == "D":
                display = "→ PRAWO"
            elif status == "WA":
                display = "↑← PRZÓD-LEWO"
            elif status == "WD":
                display = "↑→ PRZÓD-PRAWO"
            else:
                display = "■ STOP"

        # Rysuj status
        screen.fill((0, 0, 0))
        text = font.render(display, True, (0, 255, 0))
        text_rect = text.get_rect(center=(200, 150))
        screen.blit(text, text_rect)

        pygame.display.flip()
        clock.tick(30)  # 30 FPS

    # Zatrzymaj przed wyjściem
    cmd("speed", 0)
    pygame.quit()
    print("\nZatrzymano.")


if __name__ == "__main__":
    main()
