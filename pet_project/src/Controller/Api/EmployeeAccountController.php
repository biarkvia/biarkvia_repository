<?php

namespace App\Controller\Api;

use App\Entity\Account;
use App\Operation\EmployeeOperation;
use App\Operation\AccountOperation;
use App\Resource\AccountResourceFactory;
use Symfony\Bundle\FrameworkBundle\Controller\AbstractController;
use Symfony\Component\HttpFoundation\Request;
use Symfony\Component\HttpFoundation\Response;
use Symfony\Component\Routing\Annotation\Route;

#[Route('/api/employees/{employeeId}/accounts', name: 'api_accounts_')]
class EmployeeAccountController extends AbstractController
{
    private EmployeeOperation $employeeOperation;
    private AccountOperation $accountOperation;
    private AccountResourceFactory $accountResourceFactory;

    public function __construct(
        EmployeeOperation $employeeOperation,
        AccountOperation $accountOperation,
        AccountResourceFactory $accountResourceFactory
    ) {
        $this->employeeOperation = $employeeOperation;
        $this->accountOperation = $accountOperation;
        $this->accountResourceFactory = $accountResourceFactory;
    }

    #[Route('', name: 'list', methods: ['GET'])]
    public function list(int $employeeId): Response
    {
        $employee = $this->employeeOperation->findById($employeeId);
        if (!$employee) {
            return $this->json(['error' => 'Employee not found'], Response::HTTP_NOT_FOUND);
        }

        $accounts = $this->accountOperation->findByEmployee($employee);
        $resources = array_map(fn(Account $account) => $this->accountResourceFactory->fromEntity($account), $accounts);

        return $this->json(['data' => $resources, 'total' => count($resources)]);
    }

    #[Route('', name: 'create', methods: ['POST'])]
    public function create(int $employeeId, Request $request): Response
    {
        $employee = $this->employeeOperation->findById($employeeId);
        if (!$employee) {
            return $this->json(['error' => 'Employee not found'], Response::HTTP_NOT_FOUND);
        }

        $data = json_decode($request->getContent(), true);

        if (empty($data['name']) || empty($data['type'])) {
            return $this->json(['error' => 'Name and type are required'], Response::HTTP_BAD_REQUEST);
        }

        $expirationDate = null;
        if (!empty($data['expiration'])) {
            try {
                $expirationDate = new \DateTime($data['expiration']);
            } catch (\Exception) {
                return $this->json(['error' => 'Invalid expiration date format'], Response::HTTP_BAD_REQUEST);
            }
        }

        $account = new Account();
        $account->setName($data['name']);
        $account->setType($data['type']);
        $account->setExpiration($expirationDate);
        $account->setEmployee($employee);

        $this->accountOperation->store($account);

        return $this->json([
            'id' => $account->getId(),
            'name' => $account->getName(),
            'type' => $account->getType(),
            'expiration' => $account->getExpiration()?->format('Y-m-d') ?? null,
            'employeeId' => $employeeId,
        ], Response::HTTP_CREATED);
    }

    #[Route('/{id}', name: 'update', methods: ['PUT'])]
    public function update(int $employeeId, int $id, Request $request): Response
    {
        $employee = $this->employeeOperation->findById($employeeId);
        if (!$employee) {
            return $this->json(['error' => 'Employee not found'], Response::HTTP_NOT_FOUND);
        }

        $account = $this->accountOperation->findById($id);
        if (!$account || $account->getEmployee() !== $employee) {
            return $this->json(['error' => 'Account not found or does not belong to the employee'], Response::HTTP_NOT_FOUND);
        }

        $data = json_decode($request->getContent(), true);
        $account->setName($data['name'] ?? $account->getName());
        $account->setType($data['type'] ?? $account->getType());
        if (!empty($data['expiration'])) {
            try {
                $expirationDate = new \DateTime($data['expiration']);
                $account->setExpiration($expirationDate);
            } catch (\Exception) {
                return $this->json(['error' => 'Invalid expiration date format'], Response::HTTP_BAD_REQUEST);
            }
        }

        $this->accountOperation->store($account);

        return $this->json($this->accountResourceFactory->fromEntity($account));
    }

    #[Route('/{id}', name: 'delete', methods: ['DELETE'])]
    public function delete(int $employeeId, int $id): Response
    {
        $employee = $this->employeeOperation->findById($employeeId);
        $account = $this->accountOperation->findById($id);

        if (!$employee) {
            return $this->json(['error' => 'Employee not found'], Response::HTTP_NOT_FOUND);
        }

        if (!$account || $account->getEmployee() !== $employee) {
            return $this->json(['error' => 'Account not found or does not belong to the employee'], Response::HTTP_NOT_FOUND);
        }

        try {
            $this->accountOperation->delete($account);
            return $this->json(['message' => 'Account deleted'], Response::HTTP_NO_CONTENT);
        } catch (\Exception) {
            return $this->json(['error' => 'Failed to delete account'], Response::HTTP_INTERNAL_SERVER_ERROR);
        }
    }
}
