#include Minecraft.h
#include player.h
#include item.h
#include projectile.h
#include sound.h
#include entity.h

class S12 : public Item {
public:
    S12() : Item(s12, ItemType::CUSTOM) {
        maxAmmo = 5;
        currentAmmo = maxAmmo;
        fireRate = 1.5f;
        lastFireTime = 0.0f;
    }

    void onRightClick(Player &player) override {
        float currentTime = Minecraft::getTime();
        if (currentAmmo <= 0) {
            player.sendMessage("Votre S12 est vide. Rechargez.");
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
            player.sendMessage("Votre S12 est à court de munitions. Rechargez.");
        }
    }

    void reload(Player &player) {
        if (player.hasItem(ItemType::SHOTGUN_SHELL)) {
            currentAmmo = maxAmmo;
            player.removeItem(ItemType::SHOTGUN_SHELL, maxAmmo);
            player.sendMessage("Votre S12 a été rechargé avec succès.");
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
        bullet.setDamage(25); 
        bullet.setVelocity(player.getDirection() * 2.8f);
        player.playSound(SoundType::SHOTGUN_FIRE);
        player.sendMessage("Boom. Le S12 a tiré.");
    }
};

extern "C" void init() {
    Item::registerItem<S12>(s12);
}