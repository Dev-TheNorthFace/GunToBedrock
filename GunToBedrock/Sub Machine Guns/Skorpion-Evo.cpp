#include Minecraft.h
#include player.h
#include item.h
#include projectile.h
#include sound.h
#include entity.h

class SKORPION_EVO : public Item {
public:
    SKORPION_EVO() : Item(skorpion_evo, ItemType::CUSTOM) {
        maxAmmo = 30;
        currentAmmo = maxAmmo;
        fireRate = 0.1f;
        lastFireTime = 0.0f;
    }

    void onRightClick(Player &player) override {
        float currentTime = Minecraft::getTime();
        if (currentAmmo <= 0) {
            player.sendMessage("Votre SKORPION EVO est vide. Rechargez.");
            return;
        }

        if (currentTime - lastFireTime < fireRate) {
            return;
        }

        shoot(player);
        currentAmmo--;
        lastFireTime = currentTime;

        if (currentAmmo == 0) {
            player.sendMessage("Votre SKORPION EVO est à court de munitions. Rechargez.");
        }
    }

    void reload(Player &player) {
        if (player.hasItem(ItemType::BULLET)) {
            int ammoToReload = maxAmmo - currentAmmo;
            currentAmmo += ammoToReload;
            player.removeItem(ItemType::BULLET, ammoToReload);
            player.sendMessage("Votre SKORPION EVO a été rechargé avec succès.");
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
        bullet.setDamage(8);
        bullet.setVelocity(player.getDirection() * 6.0f);
        player.playSound(SoundType::GUNSHOT);
        player.sendMessage("Tir. Le SKORPION EVO a tiré.");
    }
};

extern "C" void init() {
    Item::registerItem<SKORPION_EVO>(skorpion_evo);
}