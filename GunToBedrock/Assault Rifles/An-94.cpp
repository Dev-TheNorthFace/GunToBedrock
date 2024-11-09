#include Minecraft.h
#include player.h
#include item.h
#include projectile.h
#include sound.h
#include entity.h

class AN94 : public Item {
public:
    AN94() : Item(an94, ItemType::CUSTOM) {
        maxAmmo = 30;
        currentAmmo = maxAmmo;
        fireRate = 0.1f;
        lastFireTime = 0.0f;
        burstFireTime = 0.2f;
        burstShots = 2;
        shotsFiredInBurst = 0;
    }

    void onRightClick(Player &player) override {
        float currentTime = Minecraft::getTime();
        if (currentAmmo <= 0) {
            player.sendMessage("Votre AN-94 est vide. Rechargez.");
            return;
        }

        if (shotsFiredInBurst < burstShots) {
            burstShoot(player);
            shotsFiredInBurst++;
        }

        lastFireTime = currentTime;
        if (shotsFiredInBurst >= burstShots) {
            shotsFiredInBurst = 0;
        }
    }

    void reload(Player &player) {
        if (player.hasItem(ItemType::BULLET)) {
            currentAmmo = maxAmmo;
            player.removeItem(ItemType::BULLET, maxAmmo);
            player.sendMessage("Votre AN-94 a été rechargé avec succès.");
        } else {
            player.sendMessage("Vous n'avez pas assez de munitions pour recharger.");
        }
    }

private:
    int maxAmmo;
    int currentAmmo;
    float fireRate;
    float lastFireTime;
    float burstFireTime;
    int burstShots;
    int shotsFiredInBurst;

    void burstShoot(Player &player) {
        float currentTime = Minecraft::getTime();
        if (currentTime - lastFireTime < fireRate) {
            player.sendMessage("Veuillez attendre avant de tirer à nouveau.");
            return;
        }
        shoot(player);
        if (shotsFiredInBurst < burstShots) {
            Minecraft::setTimeout([this, &player]() { burstShoot(player); }, burstFireTime);
        }
    }

    void shoot(Player &player) {
        Projectile &bullet = player.spawnProjectile(ProjectileType::SNOWBALL);
        bullet.setDamage(25);
        bullet.setVelocity(player.getDirection() * 3.0f);
        player.playSound(SoundType::SHOOT);
        player.sendMessage("Bang. L'AN-94 a tiré.");
    }
};

extern "C" void init() {
    Item::registerItem<AN94>(an94);
}