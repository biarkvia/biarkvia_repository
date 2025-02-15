<?php

namespace App\Operation;

use Doctrine\ORM\EntityManagerInterface;
use Doctrine\ORM\EntityRepository;

abstract class AbstractOperation
{
    protected EntityManagerInterface $manager;

    public function __construct(EntityManagerInterface $manager)
    {
        $this->manager = $manager;
    }

    /**
     * Get the repository for a given class.
     *
     * @template T
     * @param class-string<T> $className
     * @return EntityRepository<T>
     */
    protected function getRepository(string $className): EntityRepository
    {
        return $this->manager->getRepository($className);
    }
}
