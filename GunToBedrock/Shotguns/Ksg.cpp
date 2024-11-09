#include Minecraft.h
#include player.h
#include item.h
#include projectile.h
#include sound.h
#include entity.h

class KSG : public Item {
public:
    KSG() : Item(ksg, ItemType::CUSTOM) {
        maxAmmo = 7;
        currentAmmo = maxAmmo;
        fireRate = 1.2f;
        lastFireTime = 0.0f;
    }

    void onRightClick(Player &player) override {
        float currentTime = Minecraft::getTime();
        if (currentAmmo <= 0) {
            player.sendMessage("Votre KSG est vide. Rechargez.");
            return;
        }

        if (currentTime - lastFireTime < fireRate) {
            player.sendMessage("Veuillez attendre avant de tirer à nouveau.");
            return;
        }

        shoot(player);
        currentAmmo--;
        lastFireTime = currentTime;

        if (currentAmmo == 0) {
            player.sendMessage("Votre KSG est à court de munitions. Rechargez.");
        }
    }

    void reload(Player &player) {
        if (player.hasItem(ItemType::SHOTGUN_SHELL)) {
            currentAmmo = maxAmmo;
            player.removeItem(ItemType::SHOTGUN_SHELL, maxAmmo);
            player.sendMessage("Votre KSG a été rechargé avec succès.");
        } else {
            player.sendMessage("Vous n'avez pas assez de munitions pour recharger.");
        }
    }

private:
    int maxAmmo;
    int currentAmmo;
    float fireRate;
    float lastFireTime;

    void shoot(Player &player) {
        Projectile &bullet = player.spawnProjectile(ProjectileType::SNOWBALL);
        bullet.setDamage(30);
        bullet.setVelocity(player.getDirection() * 2.5f);
        player.playSound(SoundType::SHOTGUN_FIRE);
        player.sendMessage("Boom ! Le KSG a tiré.");
    }
};

extern "C" void init() {
    Item::registerItem<KSG>(ksg);
}